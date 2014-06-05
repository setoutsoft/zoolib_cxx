/* -------------------------------------------------------------------------------------------------
Copyright (c) 2014 Andrew Green
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

#include "zoolib/ZLog.h"
#include "zoolib/ZCompare.h"
#include "zoolib/ZMACRO_foreach.h"
#include "zoolib/ZStream_Data_T.h"
#include "zoolib/ZStrim_Stream.h"
#include "zoolib/ZStrimU_StreamUTF8Buffered.h"
#include "zoolib/ZStrimmer_Streamer.h"
#include "zoolib/ZStringf.h"
#include "zoolib/ZUtil_STL_map.h"
#include "zoolib/ZUtil_STL_vector.h"
#include "zoolib/ZUtil_Strim_IntervalTreeClock.h"
#include "zoolib/ZVisitor_Expr_Bool_ValPred_Do_GetNames.h"
#include "zoolib/ZYad_Any.h"
#include "zoolib/ZYad_ZooLibStrim.h"

#include "zoolib/RelationalAlgebra/RelHead.h"

#include "zoolib/dataspace/Searcher_DatonSet.h"

#include "zoolib/QueryEngine/ResultFromWalker.h"
#include "zoolib/QueryEngine/Walker_Project.h"
#include "zoolib/QueryEngine/Walker_Result.h"
#include "zoolib/QueryEngine/Walker_Restrict.h"

#include "zoolib/RelationalAlgebra/Util_Strim_Rel.h"
#include "zoolib/RelationalAlgebra/Util_Strim_RelHead.h"

namespace ZooLib {
namespace ZDataspace {

using ZDatonSet::DatonSet;
using ZDatonSet::Daton;
using ZDatonSet::Deltas;
using ZDatonSet::Vector_Event_Delta_t;

using namespace ZUtil_STL;

using std::make_pair;
using std::map;
using std::pair;
using std::set;
using std::vector;

namespace QE = QueryEngine;
namespace RA = RelationalAlgebra;

/*
Minimally, keep an index of property names in contents -- that way we can efficiently
identify those entities that satisfy a RelHead.
*/

static const ZStrimW& operator<<(const ZStrimW& w, const Daton& iDaton)
	{
	const ZData_Any& theData = iDaton.GetData();
	w.Write(static_cast<const UTF8*>(theData.GetPtr()), theData.GetSize());
	return w;
	}

// =================================================================================================
// MARK: - spAsVal (anonymous)

namespace { // anonymous

ZVal_Any spAsVal(const ZData_Any& iData)
	{
	try
		{
		ZRef<ZStreamerR> theStreamerR =
			new ZStreamerRPos_T<ZStreamRPos_Data_T<ZData_Any> >(iData);

		ZRef<ZStrimmerU> theStrimmerU =
			new ZStrimmerU_Streamer_T<ZStrimU_StreamUTF8Buffered>(1024, theStreamerR);

		ZRef<ZYadR> theYadR = ZYad_ZooLibStrim::sYadR(theStrimmerU);

		return sFromYadR(ZVal_Any(), theYadR);
		}
	catch (...)
		{
		return ZVal_Any();
		}
	}

} // anonymous namespace

// =================================================================================================
// MARK: - Daton/Val conversion.

ZVal_Any sAsVal(const Daton& iDaton)
	{ return spAsVal(iDaton.GetData()); }

Daton sAsDaton(const ZVal_Any& iVal)
	{
	ZData_Any theData;
	ZYad_ZooLibStrim::sToStrim(sYadR(iVal),
		ZStrimW_StreamUTF8(sStreamRWPos_Data_T(theData)));
	return theData;
	}

// =================================================================================================
// MARK: - Searcher_DatonSet::Walker

class Searcher_DatonSet::Walker
:	public QE::Walker
	{
public:
	Walker(ZRef<Searcher_DatonSet> iSearcher, const ConcreteHead& iConcreteHead)
	:	fSearcher(iSearcher)
	,	fConcreteHead(iConcreteHead)
		{}

	virtual ~Walker()
		{}

// From QE::Walker
	virtual void Rewind()
		{ fSearcher->pRewind(this); }

	virtual ZRef<QE::Walker> Prime(const map<string8,size_t>& iOffsets,
		map<string8,size_t>& oOffsets,
		size_t& ioBaseOffset)
		{
		fSearcher->pPrime(this, iOffsets, oOffsets, ioBaseOffset);
		return this;
		}

	virtual bool QReadInc(ZVal_Any* ioResults)
		{ return fSearcher->pReadInc(this, ioResults); }

	const ZRef<Searcher_DatonSet> fSearcher;
	const ConcreteHead fConcreteHead;
	size_t fBaseOffset;
	Map_Main::const_iterator fCurrent_Main;
	Map_Pending::const_iterator fCurrent_Pending;
	std::set<std::vector<ZVal_Any> > fPriors;
	};

// =================================================================================================
// MARK: - Searcher_DatonSet::ClientSearch

class Searcher_DatonSet::DLink_ClientSearch_InPSearch
:	public DListLink<ClientSearch, DLink_ClientSearch_InPSearch, kDebug>
	{};

class Searcher_DatonSet::DLink_ClientSearch_NeedsWork
:	public DListLink<ClientSearch, DLink_ClientSearch_NeedsWork, kDebug>
	{};

class Searcher_DatonSet::ClientSearch
:	public DLink_ClientSearch_InPSearch
,	public DLink_ClientSearch_NeedsWork
	{
public:
	ClientSearch(int64 iRefcon, PSearch* iPSearch)
	:	fRefcon(iRefcon)
	,	fPSearch(iPSearch)
		{}

	int64 const fRefcon;
	PSearch* const fPSearch;
	};

// =================================================================================================
// MARK: - Searcher_DatonSet::PSearch

class Searcher_DatonSet::DLink_PSearch_InPScan
:	public DListLink<PSearch, DLink_PSearch_InPScan, kDebug>
	{};

class Searcher_DatonSet::DLink_PSearch_NeedsWork
:	public DListLink<PSearch, DLink_PSearch_NeedsWork, kDebug>
	{};

class Searcher_DatonSet::PSearch
:	public DLink_PSearch_InPScan
,	public DLink_PSearch_NeedsWork
	{
public:
	PSearch(const SearchSpec& iSearchSpec)
	:	fSearchSpec(iSearchSpec)
		{}

	const SearchSpec fSearchSpec;

	DListHead<DLink_ClientSearch_InPSearch> fClientSearch_InPSearch;

	PScan* fPScan;

	ZRef<QE::Result> fResult;
	};

// =================================================================================================
// MARK: - Searcher_DatonSet::PScan

class Searcher_DatonSet::DLink_PScan_NeedsWork
:	public DListLink<PScan, DLink_PScan_NeedsWork, kDebug>
	{};

class Searcher_DatonSet::PScan
:	public DLink_PScan_NeedsWork
	{
public:
	PScan(const ConcreteHead& iConcreteHead)
	:	fConcreteHead(iConcreteHead)
		{}

	const ConcreteHead fConcreteHead;
	DListHead<DLink_PSearch_InPScan> fPSearch_InPScan;
	ZRef<QE::Result> fResult;
	};

// =================================================================================================
// MARK: - Searcher_DatonSet

//void Searcher_DatonSet::ForceUpdate()
//	{ Searcher::pTriggerResultsAvailable(); }

Searcher_DatonSet::Searcher_DatonSet(ZRef<DatonSet> iDatonSet)
:	fDatonSet(iDatonSet)
,	fEvent(Event::sZero())
	{}

Searcher_DatonSet::~Searcher_DatonSet()
	{
	for (DListEraser<PScan,DLink_PScan_NeedsWork> eraser = fPScan_NeedsWork;
		eraser; eraser.Advance())
		{}

	for (DListEraser<PSearch,DLink_PSearch_NeedsWork> eraser = fPSearch_NeedsWork;
		eraser; eraser.Advance())
		{}

	for (DListEraser<ClientSearch,DLink_ClientSearch_NeedsWork> eraser = fClientSearch_NeedsWork;
		eraser; eraser.Advance())
		{}
	}

bool Searcher_DatonSet::Intersects(const RelHead& iRelHead)
	{ return true; }

void Searcher_DatonSet::ModifyRegistrations(
	const AddedSearch* iAdded, size_t iAddedCount,
	const int64* iRemoved, size_t iRemovedCount)
	{
	ZGuardMtxR guard(fMtxR);

	for (/*no init*/; iAddedCount--; ++iAdded)
		{
		const SearchSpec& theSearchSpec = iAdded->GetSearchSpec();

		const pair<Map_SearchSpec_PSearch::iterator,bool>
			iterPSearchPair = fMap_SearchSpec_PSearch.insert(
				make_pair(theSearchSpec, PSearch(theSearchSpec)));

		PSearch* thePSearch = &iterPSearchPair.first->second;

		if (iterPSearchPair.second)
			{
			// It's a new PSearch, so we'll need to work on it
			sInsertBackMust(fPSearch_NeedsWork, thePSearch);

			// and connect it to a PScan
			RelHead theRH_Required, theRH_Optional;
			RA::sRelHeads(theSearchSpec.GetConcreteHead(), theRH_Required, theRH_Optional);

			const RelHead theRH_Restriction = sGetNames(theSearchSpec.GetRestriction());

			const ConcreteHead theCH = RA::sConcreteHead(
				theRH_Required, theRH_Optional | theRH_Restriction);

			const pair<Map_PScan::iterator,bool>
				iterPScanPair = fMap_PScan.insert(make_pair(theCH, PScan(theCH)));

			PScan* thePScan = &iterPScanPair.first->second;
			thePSearch->fPScan = thePScan;
			sInsertBackMust(thePScan->fPSearch_InPScan, thePSearch);

			if (iterPScanPair.second)
				{
				// It's a new PScan, so we'll need to work on it.
				sInsertBackMust(fPScan_NeedsWork, thePScan);
				}
			}

		const int64 theRefcon = iAdded->GetRefcon();

		const pair<map<int64,ClientSearch>::iterator,bool>
			iterClientSearchPair = fMap_Refcon_ClientSearch.insert(
				make_pair(theRefcon, ClientSearch(theRefcon, thePSearch)));
		ZAssert(iterClientSearchPair.second);

		ClientSearch* theClientSearch = &iterClientSearchPair.first->second;
		sInsertBackMust(thePSearch->fClientSearch_InPSearch, theClientSearch);

		sInsertBackMust(fClientSearch_NeedsWork, theClientSearch);
		}

	while (iRemovedCount--)
		{
		int64 theRefcon = *iRemoved++;

		map<int64, ClientSearch>::iterator iterClientSearch =
			fMap_Refcon_ClientSearch.find(theRefcon);

		ZAssertStop(kDebug, iterClientSearch != fMap_Refcon_ClientSearch.end());

		ClientSearch* theClientSearch = &iterClientSearch->second;

		PSearch* thePSearch = theClientSearch->fPSearch;
		sEraseMust(thePSearch->fClientSearch_InPSearch, theClientSearch);
		if (sIsEmpty(thePSearch->fClientSearch_InPSearch))
			{
			PScan* thePScan = thePSearch->fPScan;
			sEraseMust(thePScan->fPSearch_InPScan, thePSearch);
			if (sIsEmpty(thePScan->fPSearch_InPScan))
				{
				sQErase(fPScan_NeedsWork, thePScan);
				sEraseMust(kDebug, fMap_PScan, thePScan->fConcreteHead);
				}

			sQErase(fPSearch_NeedsWork, thePSearch);
			sEraseMust(kDebug, fMap_SearchSpec_PSearch, thePSearch->fSearchSpec);
			}

		sQErase(fClientSearch_NeedsWork, theClientSearch);
		fMap_Refcon_ClientSearch.erase(iterClientSearch);
		}

	if (sNotEmpty(fClientSearch_NeedsWork)
		|| sNotEmpty(fPSearch_NeedsWork)
		|| sNotEmpty(fPScan_NeedsWork))
		{
		guard.Release();
		Searcher::pTriggerResultsAvailable();
		}
	}

void Searcher_DatonSet::CollectResults(vector<SearchResult>& oChanged)
	{
	ZGuardMtxR guard(fMtxR);

	this->pPull();

	guard.Release();

	Searcher::pCollectResultsCalled();

	guard.Acquire();

	oChanged.clear();

	// Go through the PScans that need work, and generate any result needed.

	for (DListEraser<PScan,DLink_PScan_NeedsWork> eraser = fPScan_NeedsWork;
		eraser; eraser.Advance())
		{
		PScan* thePScan = eraser.Current();
		if (not thePScan->fResult)
			{
			ZRef<QE::Walker> theWalker = new Walker(this, thePScan->fConcreteHead);
			thePScan->fResult = QE::sResultFromWalker(theWalker);

			for (DListIterator<PSearch, DLink_PSearch_InPScan>
				iter = thePScan->fPSearch_InPScan; iter; iter.Advance())
				{
				PSearch* thePSearch = iter.Current();
				thePSearch->fResult.Clear();
				sQInsertBack(fPSearch_NeedsWork, thePSearch);
				}
			}
		}

	for (DListEraser<PSearch,DLink_PSearch_NeedsWork> eraser = fPSearch_NeedsWork;
		eraser; eraser.Advance())
		{
		PSearch* thePSearch = eraser.Current();

		if (not thePSearch->fResult)
			{
			PScan* thePScan = thePSearch->fPScan;

			const SearchSpec& theSearchSpec = thePSearch->fSearchSpec;

			const ZRef<ZExpr_Bool>& theRestriction = theSearchSpec.GetRestriction();

			ZRef<QE::Walker> theWalker;
			if (theRestriction && theRestriction != sTrue())
				{
				theWalker = new QE::Walker_Result(thePScan->fResult);
				theWalker = new QE::Walker_Restrict(theWalker, theRestriction);
				}

			const RelHead theRH_Wanted = RA::sRelHead(theSearchSpec.GetConcreteHead());
			if (theRH_Wanted != RA::sRelHead(thePScan->fConcreteHead))
				{
				if (not theWalker)
					theWalker = new QE::Walker_Result(thePScan->fResult);
				theWalker = new QE::Walker_Project(theWalker, theRH_Wanted);
				}

			if (not theWalker)
				thePSearch->fResult = thePScan->fResult;
			else
				thePSearch->fResult = QE::sResultFromWalker(theWalker);

			for (DListIterator<ClientSearch, DLink_ClientSearch_InPSearch>
				iter = thePSearch->fClientSearch_InPSearch; iter; iter.Advance())
				{ sQInsertBack(fClientSearch_NeedsWork, iter.Current()); }
			}
		}

	ZRef<Event> theEvent = fDatonSet->GetEvent();
	if (fDatonSet_Temp)
		theEvent = theEvent->Joined(fDatonSet_Temp->GetEvent());

	for (DListEraser<ClientSearch,DLink_ClientSearch_NeedsWork> eraser = fClientSearch_NeedsWork;
		eraser; eraser.Advance())
		{
		ClientSearch* theClientSearch = eraser.Current();
		PSearch* thePSearch = theClientSearch->fPSearch;
		oChanged.push_back(SearchResult(theClientSearch->fRefcon, thePSearch->fResult, theEvent));
		}

	}

ZRef<ZDatonSet::DatonSet> Searcher_DatonSet::GetDatonSet()
	{ return fDatonSet; }

ZRef<Event> Searcher_DatonSet::Insert(const Daton& iDaton)
	{
	ZGuardMtxR guard(fMtxR);
	this->pModify(iDaton, sAsVal(iDaton), true);
	ZRef<Event> result = this->pConditionalPushDown();
	guard.Release();

	Searcher::pTriggerResultsAvailable();

	return result;
	}

ZRef<Event> Searcher_DatonSet::Erase(const Daton& iDaton)
	{
	ZGuardMtxR guard(fMtxR);
	this->pModify(iDaton, sAsVal(iDaton), false);
	ZRef<Event> result = this->pConditionalPushDown();
	guard.Release();

	Searcher::pTriggerResultsAvailable();

	return result;
	}

ZRef<Event> Searcher_DatonSet::Replace(const ZDatonSet::Daton& iOld, const ZDatonSet::Daton& iNew)
	{
	ZGuardMtxR guard(fMtxR);
	this->pModify(iOld, sAsVal(iOld), false);
	this->pModify(iNew, sAsVal(iNew), true);
	ZRef<Event> result = this->pConditionalPushDown();
	guard.Release();

	Searcher::pTriggerResultsAvailable();

	return result;
	}

size_t Searcher_DatonSet::OpenTransaction()
	{
	ZAcqMtxR acq(fMtxR);
	if (fStack_Map_Pending.empty())
		fDatonSet_Temp = fDatonSet->Fork();

	fStack_Map_Pending.push_back(fMap_Pending);

	return fStack_Map_Pending.size();
	}

void Searcher_DatonSet::ClearTransaction(size_t iIndex)
	{
	ZAcqMtxR acq(fMtxR);
	ZAssert(iIndex == fStack_Map_Pending.size());

	this->pChangedAll();

	fMap_Pending = fStack_Map_Pending.back();
	}

void Searcher_DatonSet::CloseTransaction(size_t iIndex)
	{
	ZGuardMtxR guard(fMtxR);

	ZAssert(iIndex == fStack_Map_Pending.size());

	this->pChangedAll();
	fStack_Map_Pending.pop_back();
	this->pConditionalPushDown();
	if (fStack_Map_Pending.empty())
		{
		ZAssert(fDatonSet_Temp);
		fDatonSet->Join(fDatonSet_Temp);
		fDatonSet_Temp.Clear();
		}
	guard.Release();

	Searcher::pTriggerResultsAvailable();
	}

void Searcher_DatonSet::pPull()
	{
	// Get our map in sync with fDatonSet
	ZRef<Deltas> theDeltas;
	fDatonSet->GetDeltas(fEvent, theDeltas, fEvent);
	const Vector_Event_Delta_t& theVector = theDeltas->GetVector();

	foreachi (iterVector, theVector)
		{
		const ZRef<Event>& theEvent = iterVector->first;
		const map<Daton, bool>& theStatements = iterVector->second->GetStatements();

		foreachi (iterStmts, theStatements)
			{
			const Daton& theDaton = iterStmts->first;

			map<Daton, pair<ZRef<Event>, ZVal_Any> >::iterator iterMap = fMap.lower_bound(theDaton);
			if (iterMap == fMap.end() || iterMap->first != theDaton)
				{
				if (iterStmts->second)
					{
					const ZVal_Any theVal = sAsVal(theDaton);
					this->pChanged(theVal);

					fMap.insert(iterMap,
						make_pair(theDaton,
						pair<ZRef<Event>, ZVal_Any>(theEvent, theVal)));
					}
				}
			else
				{
				const bool alb = sIsBefore(iterMap->second.first, theEvent);
				//##const bool bla = sIsBefore(theEvent, iterMap->second.first);

				if (alb)
					{
					// theNamedEvent is more recent than what we've got and thus supersedes it.

					const ZVal_Any theVal = sAsVal(theDaton);
					this->pChanged(theVal);

					if (iterStmts->second)
						iterMap->second = pair<ZRef<Event>, ZVal_Any>(theEvent, theVal);
					else
						fMap.erase(iterMap);
					}
				}
			}
		}
	}

ZRef<Event> Searcher_DatonSet::pConditionalPushDown()
	{
	if (sNotEmpty(fStack_Map_Pending))
		{
		ZAssert(fDatonSet_Temp);
		return fDatonSet->GetEvent()->Joined(fDatonSet_Temp->TickleClock());
		}
	else
		{
		ZRef<DatonSet> theDS = fDatonSet_Temp ? fDatonSet_Temp : fDatonSet;
		foreachi (ii, fMap_Pending)
			{
			if (ii->second.second)
				theDS->Insert(ii->first);
			else
				theDS->Erase(ii->first);
			}
		fMap_Pending.clear();
		return theDS->GetEvent();
		}
	}

void Searcher_DatonSet::pModify(const ZDatonSet::Daton& iDaton, const ZVal_Any& iVal, bool iSense)
	{
	Map_Pending::iterator ii = fMap_Pending.find(iDaton);
	if (fMap_Pending.end() == ii)
		{
		fMap_Pending.insert(make_pair(iDaton, make_pair(iVal, iSense)));
		}
	else if (ii->second.second == iSense)
		{
		if (ZLOGF(s, eDebug))
			s << "\nDaton: " << iDaton;
		ZDebugStop(0);
		}
	else
		{
		fMap_Pending.erase(ii);
		}
	this->pChanged(iVal);
	Searcher::pTriggerResultsAvailable();
	}

void Searcher_DatonSet::pChanged(const ZVal_Any& iVal)
	{
	const ZMap_Any theMap = iVal.Get<ZMap_Any>();
	RelHead theRH;
	for (ZMap_Any::Index_t i = theMap.Begin(); i != theMap.End(); ++i)
		theRH |= RA::ColName(theMap.NameOf(i));

	// This is overkill -- we don't necessarily have to rework the whole PScan.
	for (Map_PScan::iterator iterPScan = fMap_PScan.begin();
		iterPScan != fMap_PScan.end(); ++iterPScan)
		{
		PScan* thePScan = &iterPScan->second;
		if (sIncludes(RA::sRelHead(thePScan->fConcreteHead), theRH))
			{
			thePScan->fResult.Clear();
			sQInsertBack(fPScan_NeedsWork, thePScan);
			}
		}
	}

void Searcher_DatonSet::pChangedAll()
	{
	for (Map_PScan::iterator iterPScan = fMap_PScan.begin();
		iterPScan != fMap_PScan.end(); ++iterPScan)
		{
		PScan* thePScan = &iterPScan->second;
		thePScan->fResult.Clear();
		sQInsertBack(fPScan_NeedsWork, thePScan);
		}
	}

void Searcher_DatonSet::pRewind(ZRef<Walker> iWalker)
	{
	iWalker->fCurrent_Main = fMap.begin();
	iWalker->fCurrent_Pending = fMap_Pending.begin();
	}

void Searcher_DatonSet::pPrime(ZRef<Walker> iWalker,
	const map<string8,size_t>& iOffsets,
	map<string8,size_t>& oOffsets,
	size_t& ioBaseOffset)
	{
	iWalker->fCurrent_Main = fMap.begin();
	iWalker->fCurrent_Pending = fMap_Pending.begin();
	iWalker->fBaseOffset = ioBaseOffset;
	const ConcreteHead& theConcreteHead = iWalker->fConcreteHead;
	for (ConcreteHead::const_iterator ii =
		theConcreteHead.begin(), end = theConcreteHead.end();
		ii != end; ++ii)
		{ oOffsets[ii->first] = ioBaseOffset++; }
	}

bool Searcher_DatonSet::pReadInc(ZRef<Walker> iWalker, ZVal_Any* ioResults)
	{
	const ConcreteHead& theConcreteHead = iWalker->fConcreteHead;

	while (iWalker->fCurrent_Main != fMap.end())
		{
		// Ignore anything that's in pending (for now).
		if (fMap_Pending.end() == fMap_Pending.find(iWalker->fCurrent_Main->first))
			{
			if (const ZMap_Any* theMap = iWalker->fCurrent_Main->second.second.PGet<ZMap_Any>())
				{
				bool gotAll = true;
				vector<ZVal_Any> subset;
				subset.reserve(theConcreteHead.size());
				size_t offset = iWalker->fBaseOffset;
				for (ConcreteHead::const_iterator
					ii = theConcreteHead.begin(), end = theConcreteHead.end();
					ii != end; ++ii, ++offset)
					{
					// Empty name indicates that we want the Daton itself.
					const string8& theName = ii->first;
					if (theName.empty())
						{
						const ZVal_Any& theVal = iWalker->fCurrent_Main->first;
						ioResults[offset] = theVal;
						subset.push_back(theVal);
						}
					else if (const ZVal_Any* theVal = sPGet(*theMap, theName))
						{
						ioResults[offset] = *theVal;
						subset.push_back(*theVal);
						}
					else if (not ii->second)
						{
						ioResults[offset] = Default_t();
						subset.push_back(Default_t());
						}
					else
						{
						gotAll = false;
						break;
						}
					}

				if (gotAll && sQInsert(iWalker->fPriors, subset))
					{
					++iWalker->fCurrent_Main;
					return true;
					}
				}
			}
		++iWalker->fCurrent_Main;
		}

	// Handle anything pending
	while (iWalker->fCurrent_Pending != fMap_Pending.end())
		{
		if (iWalker->fCurrent_Pending->second.second)
			{
			if (const ZMap_Any* theMap = iWalker->fCurrent_Pending->second.first.PGet<ZMap_Any>())
				{
				bool gotAll = true;
				vector<ZVal_Any> subset;
				subset.reserve(theConcreteHead.size());
				size_t offset = iWalker->fBaseOffset;
				for (ConcreteHead::const_iterator
					ii = theConcreteHead.begin(), end = theConcreteHead.end();
					ii != end; ++ii, ++offset)
					{
					const string8& theName = ii->first;
					if (theName.empty())
						{
						const ZVal_Any& theVal = iWalker->fCurrent_Pending->first;
						ioResults[offset] = theVal;
						subset.push_back(theVal);
						}
					else if (const ZVal_Any* theVal = theMap->PGet(theName))
						{
						ioResults[offset] = *theVal;
						subset.push_back(*theVal);
						}
					else if (not ii->second)
						{
						ioResults[offset] = Default_t();
						subset.push_back(Default_t());
						}
					else
						{
						gotAll = false;
						break;
						}
					}

				if (gotAll && sQInsert(iWalker->fPriors, subset))
					{
					++iWalker->fCurrent_Pending;
					return true;
					}
				}
			}
		++iWalker->fCurrent_Pending;
		}

	return false;
	}

} // namespace ZDataspace
} // namespace ZooLib