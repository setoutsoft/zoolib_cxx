/* -------------------------------------------------------------------------------------------------
Copyright (c) 2018 Andrew Green
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

#ifndef __ZooLib_PullPush_h__
#define __ZooLib_PullPush_h__ 1
#include "zconfig.h"

#include "zoolib/Any.h"
#include "zoolib/Chan.h"
#include "zoolib/ChanConnection_XX_MemoryPipe.h"
#include "zoolib/Channer.h"
#include "zoolib/ChanR.h"
#include "zoolib/ChanR_Bin.h"
#include "zoolib/ChanR_UTF.h"
#include "zoolib/ChanW.h"
#include "zoolib/Name.h"
#include "zoolib/ZQ.h"

#include <string>

namespace ZooLib {
namespace PullPush {

// =================================================================================================
#pragma mark -
#pragma mark

struct StartMap {};
extern const Any kStartMap;

struct StartSeq {};
extern const Any kStartSeq;

struct End {};
extern const Any kEnd;

} // namespace PullPush

using ChanR_Any = ChanR<Any>;
using ChanW_Any = ChanW<Any>;

using ChanRCon_Bin = ChanRCon<byte>;

using ChanWCon_Any = ChanWCon<Any>;
using ChannerWCon_Any = Channer<ChanWCon_Any>;

using ChanRCon_Any = ChanRCon<Any>;
using ChannerRCon_Any = Channer<ChanRCon_Any>;

void sPush(const Any& iVal, const ChanW_Any& iChanW);

template <class T>
void sPush(const T& iVal, const ChanW_Any& iChanW)
	{ sEWrite<Any>(iChanW, Any(iVal)); }

void sPull_UTF_Push(const ChanR_UTF& iChanR, const ChanW_Any& iChanW);
void sPull_UTF_Push(const ChanR_UTF& iChanR, uint64 iCount, const ChanW_Any& iChanW);

void sPull_Bin_Push(const ChanR_Bin& iChanR, const ChanW_Any& iChanW);
void sPull_Bin_Push(const ChanR_Bin& iChanR, uint64 iCount, const ChanW_Any& iChanW);

template <class EE>
using PipePair = std::pair<ZRef<ChannerRCon<EE>>,ZRef<ChannerWCon<EE>>>;

template <class EE>
PipePair<EE> sMakePipePair()
	{
	ZRef<Channer<ChanConnection<EE>>> thePipeChanner = new Channer_T<ChanConnection_XX_MemoryPipe<EE>>;
	return PipePair<EE>(thePipeChanner,thePipeChanner);
	}

} // namespace ZooLib

#endif // __ZooLib_PullPush_h__