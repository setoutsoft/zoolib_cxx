/* -------------------------------------------------------------------------------------------------
Copyright (c) 2010 Andrew Green
http://www.zoolib.org

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------------------------- */

#include "zoolib/ZCallable_PMF.h"
#include "zoolib/ZLog.h"
#include "zoolib/dataspace/ZDataspace_Dataspace.h"

namespace ZooLib {
namespace ZDataspace {

using std::map;
using std::pair;
using std::set;
using std::vector;

// =================================================================================================
#pragma mark -
#pragma mark * PSieve

class Dataspace::DLink_PSieve_LocalUpdate
:	public DListLink<PSieve, DLink_PSieve_LocalUpdate, kDebug>
	{};

class Dataspace::DLink_PSieve_SourceUpdate
:	public DListLink<PSieve, Dataspace::DLink_PSieve_SourceUpdate, kDebug>
	{};

class Dataspace::DLink_PSieve_Changed
:	public DListLink<PSieve, Dataspace::DLink_PSieve_Changed, kDebug>
	{};

class Dataspace::PSieve
:	public DLink_PSieve_LocalUpdate
,	public DLink_PSieve_SourceUpdate
,	public DLink_PSieve_Changed
	{
public:
	Dataspace* fDataspace;
	SearchSpec fSearchSpec;
	int64 fRefcon; // When zero, is not yet known to the Source

	DListHead<DLink_Sieve_Using> fSieves_Using;

	ZRef<SearchRows> fSearchRows_Local;
	ZRef<SearchRows> fSearchRows_Source;
	};

// =================================================================================================
#pragma mark -
#pragma mark * Dataspace

Dataspace::Dataspace(Source* iSource)
:	fSource(iSource)
,	fCalled_LocalUpdateNeeded(false)
,	fCalled_SourceUpdateNeeded(false)
,	fNextRefcon(1)
	{
	fCallable_Source = MakeCallable(&Dataspace::pCallback_Source, this);
	fSource->SetCallable(fCallable_Source);
	}

Dataspace::~Dataspace()
	{
	fSource->SetCallable(null);
	delete fSource;
	}

void Dataspace::SetCallable_LocalUpdateNeeded(ZRef<Callable_UpdateNeeded> iCallable)
	{
	ZAcqMtxR guardR_Structure(fMtx_Structure);
	fCallable_LocalUpdateNeeded = iCallable;
	}

void Dataspace::SetCallable_SourceUpdateNeeded(ZRef<Callable_UpdateNeeded> iCallable)
	{
	ZAcqMtxR guardR_Structure(fMtx_Structure);
	fCallable_SourceUpdateNeeded = iCallable;
	}

void Dataspace::Register(ZRef<Sieve> iSieve, const SearchSpec& iSearchSpec)
	{
	// Perhaps we should just put the Sieve on a to_be_registered list,
	// and do the actual hookup in Update. Otherwise there's a bit of a race
	// where we add a Sieve to a PSieve that's going to be Sync-ed for other
	// reasons, and some Sieve's may benefit, whereas others won't. That will
	// mean a newly registered Sieve has no results till the next Update.
	ZGuardRMtxR guardR_CallLocalUpdate(fMtx_CallLocalUpdate);
	ZGuardRMtxR guardR_Structure(fMtx_Structure);

	PSieve* thePSieve;
	map<SearchSpec, PSieve>::iterator position = fSearchSpec_To_PSieve.lower_bound(iSearchSpec);
	if (position != fSearchSpec_To_PSieve.end() && position->first == iSearchSpec)
		{
		thePSieve = &position->second;
		}
	else
		{
		thePSieve = &fSearchSpec_To_PSieve.
			insert(position, pair<SearchSpec, PSieve>(iSearchSpec, PSieve()))->second;

		thePSieve->fDataspace = this;
		thePSieve->fSearchSpec = iSearchSpec;
		thePSieve->fRefcon = 0;

		fPSieves_LocalUpdate.Insert(thePSieve);

		this->pTriggerLocalUpdate();
		}

	Sieve* theSieve = iSieve.Get();
	theSieve->fPSieve = thePSieve;
	thePSieve->fSieves_Using.Insert(theSieve);

	fSieves_JustRegistered.Insert(theSieve);
	}

void Dataspace::LocalUpdate()
	{
	ZAcqMtxR acq_CallLocalUpdate(fMtx_CallLocalUpdate);
	ZGuardRMtxR guardR_Structure(fMtx_Structure);

	fCalled_LocalUpdateNeeded = false;

	if (!fPSieves_LocalUpdate && !fPSieves_Changed)
		{
		return;
		}

	for (DListIteratorEraseAll<PSieve, DLink_PSieve_LocalUpdate> iter = fPSieves_LocalUpdate;
		iter; iter.Advance())
		{
		PSieve* thePSieve = iter.Current();

		if (thePSieve->fSieves_Using)
			{
			// It's in use.
			if (!thePSieve->fRefcon)
				{
				// But not known to the source.
				fPSieves_SourceUpdate.InsertIfNotContains(thePSieve);
				}
			}
		else if (thePSieve->fRefcon)
			{
			// It's not in use and is known to the source.
			fPSieves_SourceUpdate.InsertIfNotContains(thePSieve);
			}
		else
			{
			// It's not in use, is not known to the source, so we can toss it.
			fPSieves_SourceUpdate.EraseIfContains(thePSieve);
			fPSieves_Changed.EraseIfContains(thePSieve);
			ZUtil_STL::sEraseMustContain(kDebug, fSearchSpec_To_PSieve, thePSieve->fSearchSpec);
			}
		}

	set<ZRef<Sieve> > sievesLoaded;
	set<ZRef<Sieve> > sievesChanged;
	for (DListIteratorEraseAll<PSieve, DLink_PSieve_Changed> iter = fPSieves_Changed;
		iter; iter.Advance())
		{
		PSieve* thePSieve = iter.Current();
		thePSieve->fSearchRows_Local = thePSieve->fSearchRows_Source;

		for (DListIterator<Sieve, DLink_Sieve_Using>
			iter = thePSieve->fSieves_Using;
			iter; iter.Advance())
			{
			Sieve* theSieve = iter.Current();
			if (fSieves_JustRegistered.Contains(theSieve))
				{
				fSieves_JustRegistered.Erase(theSieve);
				sievesLoaded.insert(theSieve);
				}
			else
				{
				sievesChanged.insert(theSieve);
				}
			}
		}

	if (!fPSieves_SourceUpdate.Empty())
		{
		this->pTriggerSourceUpdate();
		}

	guardR_Structure.Release();

	this->Updated(sievesLoaded, sievesChanged);
	}

void Dataspace::SourceUpdate()
	{
	ZGuardRMtxR guardR_CallSourceUpdate(fMtx_CallSourceUpdate);
	ZGuardRMtxR guardR_Structure(fMtx_Structure);

	fCalled_SourceUpdateNeeded = false;

	vector<AddedSearch> addedSearches;
	vector<int64> removedQueries;

	for (DListIteratorEraseAll<PSieve, DLink_PSieve_SourceUpdate> iter = fPSieves_SourceUpdate;
		iter; iter.Advance())
		{
		PSieve* thePSieve = iter.Current();

		if (thePSieve->fSieves_Using)
			{
			// It's in use.
			if (!thePSieve->fRefcon)
				{
				// But not known to the source.
				thePSieve->fRefcon = fNextRefcon++;
				ZUtil_STL::sInsertMustNotContain(kDebug,
					fRefcon_To_PSieveStar, thePSieve->fRefcon, thePSieve);
				addedSearches.push_back(AddedSearch(thePSieve->fRefcon, thePSieve->fSearchSpec));
				}
			}
		else if (thePSieve->fRefcon)
			{
			// It's not in use, and is known to the source.
			removedQueries.push_back(thePSieve->fRefcon);
			ZUtil_STL::sEraseMustContain(kDebug, fRefcon_To_PSieveStar, thePSieve->fRefcon);
			thePSieve->fRefcon = 0;
			thePSieve->fSearchRows_Source.Clear();
			fPSieves_LocalUpdate.InsertIfNotContains(thePSieve);
			}
		else
			{
			// Shouldn't still be on the sync list if it's not in use and not known to the source
//			if (ZLOG(s, eNotice, "ZTSoup"))
//				{
//				s << "Got a PSieve on the sync list that maybe shouldn't be there: "
//					<< " On update list? "
//					<< (static_cast<DLink_PSieve_LocalUpdate*>(thePSieve)->fNext ? "yes " : "no ")
//					<< ZStringf("ID: %llX, value: ", reinterpret_cast<int64>(thePSieve))
//					<< thePSieve->fTBQuery.AsTuple();
//				}
			}
		}

	guardR_Structure.Release();

	vector<SearchResult> theChanged;
	ZRef<Event> theEvent;

	fSource->Update(
		false,
		ZUtil_STL::sFirstOrNil(addedSearches), addedSearches.size(),
		ZUtil_STL::sFirstOrNil(removedQueries), removedQueries.size(),
		theChanged,
		theEvent);

	guardR_Structure.Acquire();

	for (vector<SearchResult>::iterator i = theChanged.begin(),
		theEnd = theChanged.end();
		i != theEnd; ++i)
		{
		PSieve* thePSieve = fRefcon_To_PSieveStar[i->fRefcon];
		ZAssert(thePSieve);
		thePSieve->fSearchRows_Source = i->fSearchRows;;
		fPSieves_Changed.InsertIfNotContains(thePSieve);
		}

	guardR_CallSourceUpdate.Release();

	if (fPSieves_LocalUpdate || fPSieves_Changed)
		{
		this->pTriggerLocalUpdate();
		}
	}

void Dataspace::Updated(
	const set<ZRef<Sieve> >& iSievesLoaded,
	const set<ZRef<Sieve> >& iSievesChanged)
	{
	this->Loaded(iSievesLoaded);
	this->Changed(iSievesChanged);
	}

void Dataspace::Loaded(const set<ZRef<Sieve> >& iSieves)
	{
	for (set<ZRef<Sieve> >::const_iterator iter = iSieves.begin(), end = iSieves.end();
		iter != end; ++iter)
		{ this->Loaded(*iter); }
	}

void Dataspace::Changed(const set<ZRef<Sieve> >& iSieves)
	{
	for (set<ZRef<Sieve> >::const_iterator iter = iSieves.begin(), end = iSieves.end();
		iter != end; ++iter)
		{ this->Changed(*iter); }
	}

void Dataspace::Loaded(const ZRef<Sieve> & iSieve)
	{
	try { iSieve->Loaded(); }
	catch (...) {}
	}

void Dataspace::Changed(const ZRef<Sieve> & iSieve)
	{
	try { iSieve->Changed(); }
	catch (...) {}
	}

void Dataspace::pTriggerLocalUpdate()
	{
	ZAcqMtxR acq(fMtx_Structure);
	if (!fCalled_LocalUpdateNeeded)
		{
		fCalled_LocalUpdateNeeded = true;
		if (ZRef<Callable_UpdateNeeded> theCallable = fCallable_LocalUpdateNeeded)
			theCallable->Call(this);
		}
	}

void Dataspace::pTriggerSourceUpdate()
	{
	ZAcqMtxR acq(fMtx_Structure);
	if (!fCalled_SourceUpdateNeeded)
		{
		fCalled_SourceUpdateNeeded = true;
		if (ZRef<Callable_UpdateNeeded> theCallable = fCallable_SourceUpdateNeeded)
			theCallable->Call(this);
		}
	}

void Dataspace::pCallback_Source(Source* iSource)
	{
	ZAcqMtxR acq(fMtx_Structure);
	this->pTriggerSourceUpdate();	
	}

void Dataspace::pFinalize(Sieve* iSieve)
	{
	ZAcqMtxR acq_CallLocalUpdate(fMtx_CallLocalUpdate);
	ZAcqMtxR acq_Structure(fMtx_Structure);

	if (!iSieve->FinishFinalize())
		return;

	PSieve* thePSieve = iSieve->fPSieve;
	iSieve->fPSieve = nullptr;

	thePSieve->fSieves_Using.Erase(iSieve);

	if (!thePSieve->fSieves_Using)
		{
		fPSieves_LocalUpdate.InsertIfNotContains(thePSieve);
		this->pTriggerLocalUpdate();
		}
	}

ZRef<SearchRows> Dataspace::pGetSearchRows(Sieve* iSieve)
	{
	if (fSieves_JustRegistered.Contains(iSieve))
		return null;
	return iSieve->fPSieve->fSearchRows_Local;
	}

bool Dataspace::pIsLoaded(Sieve* iSieve)
	{ return ! fSieves_JustRegistered.Contains(iSieve); }

// =================================================================================================
#pragma mark -
#pragma mark * Sieve

Sieve::Sieve()
:	fPSieve(nullptr)
	{}

Sieve::~Sieve()
	{
	ZAssert(!fPSieve);
	}

void Sieve::Finalize()
	{
	if (fPSieve)
		fPSieve->fDataspace->pFinalize(this);
	else
		ZCounted::Finalize();
	}

void Sieve::Changed()
	{ this->Changed(false); }

void Sieve::Loaded()
	{ this->Changed(true); }

void Sieve::Changed(bool iIsLoad)
	{}

ZRef<SearchRows> Sieve::GetSearchRows()
	{
	if (fPSieve)
		return fPSieve->fDataspace->pGetSearchRows(this);
	return null;
	}

bool Sieve::IsLoaded()
	{
	if (fPSieve)
		return fPSieve->fDataspace->pIsLoaded(this);
	return false;
	}

// =================================================================================================
#pragma mark -
#pragma mark * Sieve_Callable

Sieve_Callable::Sieve_Callable(ZRef<Callable> iCallable)
:	fCallable(iCallable)
	{}

void Sieve_Callable::Changed(bool iIsLoad)
	{
	if (ZRef<Callable> theCallable = fCallable)
		theCallable->Call(this, iIsLoad);
	}

} // namespace ZDataspace
} // namespace ZooLib