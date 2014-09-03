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

#ifndef __ZooLib_Util_Chan_h__
#define __ZooLib_Util_Chan_h__ 1
#include "zconfig.h"

#include "zoolib/ChanR.h"
#include "zoolib/ChanW.h"

namespace ZooLib {
namespace Util_Chan {

// =================================================================================================
// MARK: -

template <class Elmt>
std::pair<uint64,uint64> sCopyFully(
	const ChanR<Elmt>& iChanR, const ChanW<Elmt>& iChanW, uint64 iCount)
	{
	Elmt buf[sStackBufferSize / sizeof(Elmt)];

	for (uint64 countRemaining = iCount; /*no test*/; /*no inc*/)
		{
		if (const size_t countRead = sRead(
			buf,
			std::min<size_t>(countRemaining, countof(buf)),
			iChanR))
			{
			const size_t countWritten = sWriteFully(buf, countRead, iChanW);

			if (countWritten == countRead)
				{
				countRemaining -= countRead;
				// Here's where we return to the beginning of the loop.
				// In all other cases we exit.
				continue;
				}

			return std::pair<uint64,uint64>(
				iCount - countRemaining + countRead,
				iCount - countRemaining + countWritten);
			}

		return std::pair<uint64,uint64>(
			iCount - countRemaining,
			iCount - countRemaining);
		}
	}

template <class Elmt>
std::pair<uint64,uint64> sCopyAll(const ChanR<Elmt>& iChanR, const ChanW<Elmt>& iChanW)
	{
	Elmt buf[sStackBufferSize / sizeof(Elmt)];

	uint64 totalCopied = 0;
	for (;;)
		{
		if (const size_t countRead = sRead(buf, countof(buf), iChanR))
			{
			const size_t countWritten = sWriteFully(buf, countRead, iChanW);

			if (countWritten == countRead)
				{
				totalCopied += countRead;
				continue;
				}

			return std::pair<uint64,uint64>(totalCopied + countRead, totalCopied + countWritten);
			}

		return std::pair<uint64,uint64>(totalCopied, totalCopied);
		}
	}

} // namespace Util_Chan

using namespace Util_Chan;

} // namespace ZooLib

#endif // __ZooLib_Util_Chan_h__