/* -------------------------------------------------------------------------------------------------
Copyright (c) 2004 Andrew Green and Learning in Motion, Inc.
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

#ifndef __ZooLib_Chan_XX_Count_h__
#define __ZooLib_Chan_XX_Count_h__ 1
#include "zconfig.h"

#include "zoolib/ChanR.h"
#include "zoolib/ChanW.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark ChanR_XX_Count

template <class EE>
class ChanR_XX_Count
:	public ChanR<EE>
	{
public:
	ChanR_XX_Count(const ChanR<EE>& iChanR)
	:	fChanR(iChanR)
	,	fCount(0)
		{}

// From ChanR
	virtual size_t QRead(EE* oDest, size_t iCount)
		{
		const size_t countRead = sQRead(fChanR, oDest, iCount);
		fCount += countRead;
		return countRead;
		}

	virtual uint64 Skip(uint64 iCount)
		{
		const size_t countSkipped = sSkip(fChanR, iCount);
		fCount += countSkipped;
		return countSkipped;
		}

	virtual size_t Readable()
		{ return sReadable(fChanR); }

// Our protocol
	uint64 GetCount()
		{ return fCount; }

protected:
	const ChanR<EE>& fChanR;
	uint64 fCount;
	};

// =================================================================================================
#pragma mark -
#pragma mark ChanW_XX_Count

template <class EE>
class ChanW_XX_Count
:	public ChanW<EE>
	{
public:
	ChanW_XX_Count(const ChanW<EE>& iChanW)
	:	fChanW(iChanW)
	,	fCount(0)
		{}

// From ChanW
	virtual size_t QWrite(const EE* iSource, size_t iCount)
		{
		const size_t countWritten = sQWrite(iSource, iCount, fChanW);
		fCount += countWritten;
		return countWritten;
		}

	virtual void Flush()
		{ sFlush(fChanW); }

// Our protocol
	uint64 GetCount()
		{ return fCount; }

protected:
	const ChanW<EE>& fChanW;
	uint64 fCount;
	};

} // namespace ZooLib

#endif // __ZooLib_Chan_XX_Count_h__