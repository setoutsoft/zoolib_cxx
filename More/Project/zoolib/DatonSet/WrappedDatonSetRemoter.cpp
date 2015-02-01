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

#include "zoolib/Callable_PMF.h"
#include "zoolib/Log.h"
#include "zoolib/Stringf.h"
#include "zoolib/Util_Any.h"
#include "zoolib/Util_Any_JSON.h"
#include "zoolib/Util_STL_map.h"
#include "zoolib/Util_STL_set.h"
#include "zoolib/Val_Any.h"
#include "zoolib/Yad_Any.h"
#include "zoolib/Yad_JSONB.h"

#include "zoolib/ZMACRO_foreach.h"

//###include "zoolib/ZStrim_Stream.h"
//###include "zoolib/ZStrimmer_Streamer.h"

#include "zoolib/DatonSet/DatonSet.h"
#include "zoolib/DatonSet/WrappedDatonSetRemoter.h"

namespace ZooLib {
namespace DatonSet {

using namespace Util_STL;
using std::set;
using std::swap;

/*
Three messages, and maybe a keepalive.

X When our fCallable_PullSuggested_Self is invoked, we send a "PullSuggested" message to
  the other side.

X When we receive a "PullSuggested" message we call fCallable_PullSuggested_Other, passing
  our fCallable_PullFrom_Self.

X When our fCallable_PullFrom_Self is invoked, we send a "PullFrom" message to the other side,
  and XXX block waiting for our reader to do its thing with "PullFromResponse". I think we can
  safely bail after a timeout if we want.

X When we receive a "PullFrom" message, we invoke each of the fCallable_PullFroms with the Event,
  and send the info back over the wire in a "PullFromResponse".

X When we get a "PullFromResponse" message, we get that data across to the writer thread
  at XXX (above). If we don't have an XXX, do nothing.

If we feel like it, send a ping.

If we haven't heard anything in a while, drop the connection.

We could decouple received PullSuggested and sends of "PullFrom", so callers aren't
blocked by network latency.
*/

static void spSendMessage(const Map_Any& iMessage, const ChanW_Bin& iChanW)
	{
	const ZTime start = ZTime::sSystem();
	Yad_JSONB::sToChan(sYadR(iMessage), iChanW);
//	ZUtil_Any_JSON::sWrite(iMessage, ZStrimW_StreamUTF8(iChanW));
	sFlush(iChanW);
	if (ZLOGF(w, eDebug+1))
		{
		w << "Sent in " << sStringf("%.3gms: ", (ZTime::sSystem() - start) * 1e3);
//		w << iMessage.Get<string8>("What");
		Util_Any_JSON::sWrite(iMessage, w);
		}
	}

static Map_Any spReadMessage(const ZRef<ChannerR_Bin>& iChannerR)
	{
	const ZTime start = ZTime::sSystem();
	ZQ<Val_Any> theQ = Yad_Any::sQFromYadR(Yad_JSONB::sYadR(iChannerR));
	if (not theQ)
		sThrow_ExhaustedR();

	const Map_Any result = theQ->Get<Map_Any>();
	if (ZLOGF(w, eDebug+1))
		{
		w << "Received in " << sStringf("%.3gms: ", (ZTime::sSystem() - start) * 1e3);
//		w << result.Get<string8>("What");
		Util_Any_JSON::sWrite(result, w);
		}
	return result;
	}

static ZRef<Event> spEventFromSeq(const Seq_Any& iSeq)
	{
	const size_t theValue = sCoerceInt(iSeq[0]);
	ZRef<Event> theLeft, theRight;
	if (iSeq.Count() > 1)
		{
		theLeft = spEventFromSeq(iSeq.Get<Seq_Any>(1));
		theRight = spEventFromSeq(iSeq.Get<Seq_Any>(2));
		}
	return Event::sMake(theValue, theLeft, theRight);
	}

static Seq_Any spSeqFromEvent(const ZRef<Event>& iEvent)
	{
	Seq_Any theSeq;
	theSeq.Append(iEvent->Value());
	if (ZRef<Event> theLeft = iEvent->Left())
		{
		theSeq.Append(spSeqFromEvent(theLeft));
		theSeq.Append(spSeqFromEvent(iEvent->Right()));
		}
	return theSeq;
	}

static ZRef<Delta> spDeltaFromSeq(const Seq_Any& iSeq)
	{
	Delta::Statements_t theStatements_t;
	foreachi (iter, iSeq)
		{
		const Seq_Any& theStatementSeq = iter->Get<Seq_Any>();
		const bool theBool = theStatementSeq[0].Get<bool>();
		const Daton& theDaton = theStatementSeq[1].Get<Data_Any>();
		sInsertMust(theStatements_t, theDaton, theBool);
		}
	return new Delta(*&theStatements_t);
	}

static Seq_Any spSeqFromDelta(const ZRef<Delta>& iDelta)
	{
	Seq_Any theSeq;

	foreachi (iter, iDelta->GetStatements())
		{
		Seq_Any& theStatementSeq = theSeq.Mut<Seq_Any>(theSeq.Count());
		theStatementSeq.Append(iter->second);
		theStatementSeq.Append(iter->first.GetData());
		}
	return theSeq;
	}

// =================================================================================================
#pragma mark -
#pragma mark WrappedDatonSetRemoter

WrappedDatonSetRemoter::WrappedDatonSetRemoter(
	const ZRef<Factory_ChannerComboRW_Bin>& iFactory,
	const ZRef<Callable_PullSuggested>& iCallable_PullSuggested_Other)
:	fFactory(iFactory)
,	fCallable_PullSuggested_Other(iCallable_PullSuggested_Other)
,	fConnectionBusy(false)
,	fPullFromPointer(nullptr)
	{
	ZAssert(fFactory);
	}

WrappedDatonSetRemoter::WrappedDatonSetRemoter(
	const ChannerComboRW_Bin& iChannerComboRW,
	const ZRef<Callable_PullSuggested>& iCallable_PullSuggested_Other)
:	fCallable_PullSuggested_Other(iCallable_PullSuggested_Other)
,	fChannerComboQ(iChannerComboRW)
,	fConnectionBusy(false)
,	fPullFromPointer(nullptr)
	{}

void WrappedDatonSetRemoter::Initialize()
	{
	ZCounted::Initialize();

	fCallable_PullSuggested_Self =
		sCallable(sWeakRef(this), &WrappedDatonSetRemoter::pPullSuggested);

	fCallable_PullFrom_Self =
		sCallable(sWeakRef(this), &WrappedDatonSetRemoter::pPullFrom);

	ZThread::sStart_T<ZRef<WrappedDatonSetRemoter> >(&WrappedDatonSetRemoter::spRead, this);
	}

ZRef<Callable_PullSuggested> WrappedDatonSetRemoter::GetCallable_PullSuggested()
	{ return fCallable_PullSuggested_Self; }

void WrappedDatonSetRemoter::pPullSuggested(const ZRef<Callable_PullFrom>& iCallable_PullFrom)
	{
	// iCallable_PullFrom will be the counterpart to fCallable_PullSuggested_Other.

	{
	ZGuardMtxR guard(fMtxR);
	sInsert(fCallables_PullFrom, iCallable_PullFrom);
	}

	for (;;)
		{
		try
			{
			ZQ<ChannerComboRW_Bin> theChannerComboQ = this->pQEnsureChannerCombo();

			ZGuardMtxR guard(fMtxR);

			if (not theChannerComboQ)
				break;

			const ChanW_Bin& theChanW = sGetChan(theChannerComboQ->GetW());

			while (fConnectionBusy)
				fCnd.Wait(fMtxR);

			Map_Any theMessage;
			theMessage.Set("What", "PullSuggested");

			{
			SaveSetRestore<bool> theSSR(fConnectionBusy, true);
			ZRelGuardR rel2(guard);

			spSendMessage(theMessage, theChanW);
			}

			fCnd.Broadcast();
			break;
			}
		catch (...)
			{
			ZGuardMtxR guard(fMtxR);
			sClear(fChannerComboQ);
			}
		}

	fCnd.Broadcast();
	}

ZRef<Deltas> WrappedDatonSetRemoter::pPullFrom(ZRef<Event> iEvent)
	{
	for (;;)
		{
		try
			{
			ZQ<ChannerComboRW_Bin> theChannerComboQ = this->pQEnsureChannerCombo();

			ZGuardMtxR guard(fMtxR);

			if (not theChannerComboQ)
				break;

			const ChanW_Bin& theChanW = sGetChan(theChannerComboQ->GetW());

			while (fConnectionBusy)
				fCnd.Wait(fMtxR);

			Map_Any theMessage;
			theMessage.Set("What", "PullFrom");
			theMessage.Set("Event", spSeqFromEvent(iEvent));

			ZRef<Deltas> theDeltas;
			fPullFromPointer = &theDeltas;

			{
			SaveSetRestore<bool> theSSR(fConnectionBusy, true);
			ZRelGuardR rel(guard);

			spSendMessage(theMessage, theChanW);
			}

			fCnd.Broadcast();

			// Wait till a PullFromResponse has come in (any PullfromResponse?), and
			while (fPullFromPointer)
				fCnd.Wait(fMtxR);

			return theDeltas;
			}
		catch (...)
			{
			ZGuardMtxR guard(fMtxR);
			sClear(fChannerComboQ);
			}
		}
	// Never get here.
	return null;
	}

void WrappedDatonSetRemoter::pRead()
	{
	ZThread::sSetName("WrappedDatonSetRemoter");

	for (;;)
		{
		try
			{
			ZQ<ChannerComboRW_Bin> theChannerComboQ = this->pQEnsureChannerCombo();

			ZGuardMtxR guard(fMtxR);

			if (not theChannerComboQ)
				{
				// No Channer was available
				if (fFactory)
					continue;
				// There's no factory to make one, bail
				break;
				}

			ZRelGuardR rel(guard);

			const Map_Any theMessage = spReadMessage(theChannerComboQ->GetR());
			const string8& theWhat = theMessage.Get<string8>("What");

			if (false)
				{}
			else if (theWhat == "PullSuggested")
				{
				try { sCall(fCallable_PullSuggested_Other, fCallable_PullFrom_Self); }
				catch (...) {}
				}
			else if (theWhat == "PullFrom")
				{
				using namespace DatonSet;

				ZRef<Event> theEvent = spEventFromSeq(theMessage.Get<Seq_Any>("Event"));

				ZAcqGuardR acq(guard);

				set<ZRef<Callable_PullFrom> > theCallables_PullFrom;
				swap(theCallables_PullFrom, fCallables_PullFrom);

				ZRelGuardR rel(guard);

				Map_Any theMessage;
				theMessage.Set("What", "PullFromResponse");
				Seq_Any& theDeltasSeq = theMessage.Mut<Seq_Any>("Deltas");

				foreachv (ZRef<Callable_PullFrom> theCallable, theCallables_PullFrom)
					{
					ZRef<Deltas> theDeltas = theCallable->Call(theEvent);

					foreachi (iterDelta, theDeltas->GetVector())
						{
						Map_Any& theEntry = theDeltasSeq.Mut<Map_Any>(theDeltasSeq.Count());
						theEntry.Set("Event", spSeqFromEvent(iterDelta->first));
						theEntry.Set("Delta", spSeqFromDelta(iterDelta->second));
						}
					}

				ZAcqGuardR acq2(guard);

				while (fConnectionBusy)
					fCnd.Wait(fMtxR);

				{
				SaveSetRestore<bool> theSSR(fConnectionBusy, true);
				ZRelGuardR rel2(guard);

				spSendMessage(theMessage, sGetChan(theChannerComboQ->GetW()));
				}

				fCnd.Broadcast();
				}
			else if (theWhat == "PullFromResponse")
				{
				using namespace DatonSet;
				// We should be blocked up in pPullFrom, get the data across from here to there
				Vector_Event_Delta_t theVED;
				foreachi (iter, theMessage.Get<Seq_Any>("Deltas"))
					{
					const Map_Any theEntry = iter->Get<Map_Any>();
					ZRef<Event> anEvent = spEventFromSeq(theEntry.Get<Seq_Any>("Event"));
					ZRef<Delta> aDelta = spDeltaFromSeq(theEntry.Get<Seq_Any>("Delta"));
					theVED.push_back(Event_Delta_t(anEvent, aDelta));
					}

				ZAcqGuardR acq(guard);
				if (fPullFromPointer)
					{
					*fPullFromPointer = new Deltas(*&theVED);
					fPullFromPointer = nullptr;
					fCnd.Broadcast();
					}
				}
			}
		catch (...)
			{
			ZGuardMtxR guard(fMtxR);
			sClear(fChannerComboQ);
			}
		}
	}

void WrappedDatonSetRemoter::spRead(ZRef<WrappedDatonSetRemoter> iWDSR)
	{ iWDSR->pRead(); }

ZQ<ChannerComboRW_Bin> WrappedDatonSetRemoter::pQEnsureChannerCombo()
	{
	ZGuardMtxR guard(fMtxR);

	if (not fChannerComboQ)
		{
		guard.Release();
		ZQ<ChannerComboRW_Bin> theChannerComboQ = sCall(fFactory);
		guard.Acquire();
		fChannerComboQ = theChannerComboQ;
		}
	return fChannerComboQ;
	}

} // namespace DatonSet
} // namespace ZooLib