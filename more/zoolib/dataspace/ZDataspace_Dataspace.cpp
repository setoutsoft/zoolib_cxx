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
#include "zoolib/ZUtil_STL_map.h"
#include "zoolib/dataspace/ZDataspace_Dataspace.h"

namespace ZooLib {
namespace ZDataspace {

using std::map;
using std::set;
using std::vector;

// =================================================================================================
#pragma mark -
#pragma mark * Dataspace

Dataspace::Dataspace(ZRef<Source> iSource)
:	fSource(iSource)
,	fCallable_Source(MakeCallable(this, &Dataspace::pCallback_Source))
,	fCalled_UpdateNeeded(false)
,	fNextRefcon(1)
	{
	fSource->SetCallable_ResultsAvailable(fCallable_Source);
	}

Dataspace::~Dataspace()
	{
	fSource->SetCallable_ResultsAvailable(null);
	}

void Dataspace::SetCallable_UpdateNeeded(ZRef<Callable_UpdateNeeded> iCallable)
	{
	ZAcqMtxR acq(fMtxR);
	fCallable_UpdateNeeded = iCallable;
	}

void Dataspace::Register(ZRef<Sieve> iSieve, const ZRef<ZRA::Expr_Rel>& iRel)
	{
	ZAcqMtxR acq(fMtxR);
	ZAssert(!iSieve->fDataspace);
	ZAssert(!iSieve->fRefcon);

	iSieve->fDataspace = this;
	iSieve->fRefcon = fNextRefcon++;
	ZUtil_STL::sInsertMustNotContain(kDebug, fMap_RefconToSieve, iSieve->fRefcon, iSieve.Get());

	const AddedSearch theAddedSearch(iSieve->fRefcon, iRel);
	fSource->ModifyRegistrations(&theAddedSearch, 1, nullptr, 0);
	}

void Dataspace::Update()
	{
	vector<SearchResult> theSearchResults;
	fSource->CollectResults(theSearchResults);

	set<ZRef<Sieve> > sievesChanged;
	set<ZRef<Sieve> > sievesLoaded;

	{
	ZAcqMtxR acq(fMtxR);
	fCalled_UpdateNeeded = false;

	if (theSearchResults.empty())
		return;
	
	for (vector<SearchResult>::iterator iterSearchResults = theSearchResults.begin();
		iterSearchResults != theSearchResults.end(); ++iterSearchResults)
		{
		Map_RefconToSieve::iterator iterSieve =
			fMap_RefconToSieve.find(iterSearchResults->GetRefcon());
		
		if (fMap_RefconToSieve.end() == iterSieve)
			continue;

		ZRef<Sieve> theSieve = iterSieve->second;
		if (theSieve->fResult)
			sievesChanged.insert(theSieve);
		else
			sievesLoaded.insert(theSieve);

		theSieve->fResult = iterSearchResults->GetResult();
		}
	}

	this->Updated(sievesLoaded, sievesChanged);
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

void Dataspace::Loaded(const ZRef<Sieve>& iSieve)
	{
	try { iSieve->Loaded(); }
	catch (...) {}
	}

void Dataspace::Changed(const ZRef<Sieve>& iSieve)
	{
	try { iSieve->Changed(); }
	catch (...) {}
	}

void Dataspace::pCallback_Source(ZRef<Source> iSource)
	{
	ZGuardRMtxR guard(fMtxR);
	if (ZRef<Callable_UpdateNeeded> theCallable = fCallable_UpdateNeeded)
		{
		if (!fCalled_UpdateNeeded)
			{
			fCalled_UpdateNeeded = true;
			guard.Release();
			theCallable->Call(this);
			}
		}
	}

void Dataspace::pFinalize(Sieve* iSieve)
	{
	ZGuardRMtxR guard(fMtxR);

	if (!iSieve->FinishFinalize())
		return;

	ZUtil_STL::sEraseMustContain(kDebug, fMap_RefconToSieve, iSieve->fRefcon);
	
	int64 const theRefcon = iSieve->fRefcon;
	delete iSieve;
	guard.Release();

	fSource->ModifyRegistrations(nullptr, 0, &theRefcon, 1);
	}

// =================================================================================================
#pragma mark -
#pragma mark * Sieve

Sieve::Sieve()
:	fDataspace(nullptr)
,	fRefcon(0)
	{}

Sieve::~Sieve()
	{}

void Sieve::Finalize()
	{
	if (fDataspace)
		fDataspace->pFinalize(this);
	else
		ZCounted::Finalize();
	}

void Sieve::Changed()
	{ this->Changed(false); }

void Sieve::Loaded()
	{ this->Changed(true); }

void Sieve::Changed(bool iIsLoad)
	{}

ZRef<ZQE::Result> Sieve::GetResult()
	{ return fResult; }

bool Sieve::IsLoaded()
	{ return fResult; }

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
