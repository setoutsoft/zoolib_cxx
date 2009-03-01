/* -------------------------------------------------------------------------------------------------
Copyright (c) 2009 Andrew Green
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

#include "zoolib/ZStrim_CFString.h"

#if ZCONFIG_SPI_Enabled(CFType)

#include "zoolib/ZMemory.h"

using std::min;
using std::max;

NAMESPACE_ZOOLIB_BEGIN

static const size_t kBufSize = sStackBufferSize;

// =================================================================================================
#pragma mark -
#pragma mark * ZStrimR_CFString

ZStrimR_CFString::ZStrimR_CFString(CFStringRef iStringRef)
:	fStringRef(iStringRef),
	fPosition(0)
	{}

ZStrimR_CFString::~ZStrimR_CFString()
	{}

void ZStrimR_CFString::Imp_ReadUTF32(UTF32* iDest, size_t iCount, size_t* oCount)
	{
	const CFIndex length = ::CFStringGetLength(fStringRef);
	if (0 == length)
		{
		if (oCount)
			*oCount = 0;
		}
	else
		{
		size_t countConsumed;
		if (const UTF16* start = (const UTF16*)::CFStringGetCharactersPtr(fStringRef))
			{
			ZUnicode::sUTF16ToUTF32(
				start + fPosition, length - fPosition,
				&countConsumed, nil,
				iDest, iCount,
				oCount);
			}
		else
			{
			UTF16 buffer[kBufSize];
			const size_t cuToCopy = min(iCount, kBufSize);
			::CFStringGetCharacters(fStringRef, CFRangeMake(fPosition, cuToCopy), (UniChar*)buffer);
			ZUnicode::sUTF16ToUTF32(
				buffer, cuToCopy,
				&countConsumed, nil,
				iDest, iCount,
				oCount);
			}
		fPosition += countConsumed;
		}
	}

void ZStrimR_CFString::Imp_ReadUTF16(UTF16* iDest,
	size_t iCountCU, size_t* oCountCU, size_t iCountCP, size_t* oCountCP)
	{
	const CFIndex length = ::CFStringGetLength(fStringRef);
	if (0 == length)
		{
		if (oCountCP)
			*oCountCP = 0;
		if (oCountCU)
			*oCountCU = 0;
		}
	else
		{
		size_t countConsumed;
		if (const UTF16* start = (const UTF16*)::CFStringGetCharactersPtr(fStringRef))
			{
			ZUnicode::sUTF16ToUTF16(
				start + fPosition, length - fPosition,
				&countConsumed, nil,
				iDest, iCountCU,
				oCountCU,
				iCountCP, oCountCP);
			}
		else
			{
			UTF16 buffer[kBufSize];
			const size_t cuToCopy = min(iCountCU, kBufSize);
			::CFStringGetCharacters(fStringRef, CFRangeMake(fPosition, cuToCopy), (UniChar*)buffer);
			ZUnicode::sUTF16ToUTF16(
				buffer, cuToCopy,
				&countConsumed, nil,
				iDest, iCountCU,
				oCountCU,
				iCountCP, oCountCP);
			}
		fPosition += countConsumed;
		}
	}

void ZStrimR_CFString::Imp_ReadUTF8(UTF8* iDest,
	size_t iCountCU, size_t* oCountCU, size_t iCountCP, size_t* oCountCP)
	{
	const CFIndex length = ::CFStringGetLength(fStringRef);
	if (0 == length)
		{
		if (oCountCP)
			*oCountCP = 0;
		if (oCountCU)
			*oCountCU = 0;
		}
	else
		{
		size_t countConsumed;
		if (const UTF16* start = (const UTF16*)::CFStringGetCharactersPtr(fStringRef))
			{
			ZUnicode::sUTF16ToUTF8(
				start + fPosition, length - fPosition,
				&countConsumed, nil,
				iDest, iCountCU,
				oCountCU,
				iCountCP, oCountCP);
			}
		else
			{
			UTF16 buffer[kBufSize];
			const size_t cuToCopy = min(iCountCU, kBufSize);
			::CFStringGetCharacters(fStringRef, CFRangeMake(fPosition, cuToCopy), (UniChar*)buffer);
			ZUnicode::sUTF16ToUTF8(
				buffer, cuToCopy,
				&countConsumed, nil,
				iDest, iCountCU,
				oCountCU,
				iCountCP, oCountCP);
			}
		fPosition += countConsumed;
		}
	}

// =================================================================================================
#pragma mark -
#pragma mark * ZStrimW_CFString

ZStrimW_CFString::ZStrimW_CFString(CFMutableStringRef iStringRef)
:	fStringRef(iStringRef)
	{}

ZStrimW_CFString::~ZStrimW_CFString()
	{}

void ZStrimW_CFString::Imp_WriteUTF16(const UTF16* iSource, size_t iCountCU, size_t* oCountCU)
	{
	if (iCountCU)
		::CFStringAppendCharacters(fStringRef, (const UniChar*)iSource, iCountCU);

	if (oCountCU)
		*oCountCU = iCountCU;
	}

NAMESPACE_ZOOLIB_END

#endif // ZCONFIG_SPI_Enabled(CFType)
