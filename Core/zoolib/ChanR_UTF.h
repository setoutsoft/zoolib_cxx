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

#ifndef __ZooLib_ChanR_UTF_h__
#define __ZooLib_ChanR_UTF_h__ 1
#include "zconfig.h"

#include "zoolib/ChanR.h"
#include "zoolib/UnicodeString.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark ChanR_UTF

typedef ChanR<UTF32> ChanR_UTF;

// =================================================================================================
#pragma mark -

void sRead(const ChanR_UTF& iChanR,
	UTF32* oDest,
	size_t iCountCU, size_t* oCountCU, size_t iCountCP, size_t* oCountCP);

void sRead(const ChanR_UTF& iChanR,
	UTF16* oDest,
	size_t iCountCU, size_t* oCountCU, size_t iCountCP, size_t* oCountCP);

void sRead(const ChanR_UTF& iChanR,
	UTF8* oDest,
	size_t iCountCU, size_t* oCountCU, size_t iCountCP, size_t* oCountCP);

// =================================================================================================
#pragma mark -

ZQ<string32> sQReadUTF32(const ChanR_UTF& iChanR, size_t iCountCP);
ZQ<string16> sQReadUTF16(const ChanR_UTF& iChanR, size_t iCountCP);
ZQ<string8> sQReadUTF8(const ChanR_UTF& iChanR, size_t iCountCP);

string32 sEReadUTF32(const ChanR_UTF& iChanR, size_t iCountCP);
string16 sEReadUTF16(const ChanR_UTF& iChanR, size_t iCountCP);
string8 sEReadUTF8(const ChanR_UTF& iChanR, size_t iCountCP);

string8 sReadAllUTF8(const ChanR_UTF& iChanR);

// =================================================================================================
#pragma mark -
#pragma mark ChanR_UTF_Native16

class ChanR_UTF_Native16
:	public ChanR_UTF
	{
public:
// From ChanR_UTF
	virtual size_t Read(UTF32* oDest, size_t iCountCU);

// Our protocol
	virtual void ReadUTF16(UTF16* oDest,
		size_t iCountCU, size_t* oCountCU, size_t iCountCP, size_t* oCountCP) = 0;
	};

// =================================================================================================
#pragma mark -
#pragma mark ChanR_UTF_Native8

class ChanR_UTF_Native8
:	public ChanR_UTF
	{
public:
// From ChanR_UTF
	virtual size_t Read(UTF32* oDest, size_t iCountCU);

// Our protocol
	virtual void ReadUTF8(UTF8* oDest,
		size_t iCountCU, size_t* oCountCU, size_t iCountCP, size_t* oCountCP) = 0;
	};

} // namespace ZooLib

#endif // __ZooLib_ChanR_UTF_h__
