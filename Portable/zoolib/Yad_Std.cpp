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

#include "zoolib/Yad_Std.h"

namespace ZooLib {

using std::string;

// =================================================================================================
#pragma mark -
#pragma mark YadParseException_Std

YadParseException_Std::YadParseException_Std(const string& iWhat)
:	YadParseException(iWhat)
	{}

YadParseException_Std::YadParseException_Std(const char* iWhat)
:	YadParseException(iWhat)
	{}

// =================================================================================================
#pragma mark -
#pragma mark ChanR_RefYad_Std

ChanR_RefYad_Std::ChanR_RefYad_Std()
:	fStarted(false)
	{}

size_t ChanR_RefYad_Std::Read(RefYad* oDest, size_t iCount)
	{
	// It is *crucial* that we read one Yad at a time. It is likely that the underlying entity is
	// in a partially consumed state, and walking the one yad we return will advance that state
	// to the point when there *would* be another yad to read.
	if (iCount && (*oDest = this->pReadInc()))
		return 1;
	return 0;
	}

ZRef<YadR> ChanR_RefYad_Std::pReadInc()
	{
	fValue.Clear();
	this->Imp_ReadInc(not sGetSet(fStarted, true), fValue);
	return fValue;
	}

// =================================================================================================
#pragma mark -
#pragma mark ChanR_NameRefYad_Std

ChanR_NameRefYad_Std::ChanR_NameRefYad_Std()
:	fStarted(false)
	{}

ChanR_NameRefYad_Std::ChanR_NameRefYad_Std(bool iFinished)
:	fStarted(iFinished)
	{}

size_t ChanR_NameRefYad_Std::Read(NameRefYad* oDest, size_t iCount)
	{
	// It is *crucial* that we read one Yad at a time. It is likely that the underlying entity is
	// in a partially consumed state, and walking the one yad we return will advance that state
	// to the point when there *would* be another yad to read.
	if (iCount)
		{
		Name theName;
		if (ZRef<YadR> theYad = this->pReadInc(theName))
			{
			oDest->first = theName;
			oDest->second = theYad;
			return 1;
			}
		}
	return 0;
	}

ZRef<YadR> ChanR_NameRefYad_Std::pReadInc(Name& oName)
	{
	oName.Clear();
	fValue.Clear();
	this->Imp_ReadInc(not sGetSet(fStarted, true), oName, fValue);

	return fValue;
	}

} // namespace ZooLib
