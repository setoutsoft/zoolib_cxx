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

#ifndef __ZooLib_Yad_Std_h__
#define __ZooLib_Yad_Std_h__ 1
#include "zconfig.h"

#include "zoolib/Any.h"
#include "zoolib/Yad.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark YadParseException_Std

class YadParseException_Std : public YadParseException
	{
public:
	YadParseException_Std(const std::string& iWhat);
	YadParseException_Std(const char* iWhat);
	};

// =================================================================================================
#pragma mark -
#pragma mark ChanR_RefYad_Std

class ChanR_RefYad_Std
:	public ChanR_RefYad
	{
public:
	ChanR_RefYad_Std();

// From ChanR_RefYad
	virtual size_t Read(RefYad* oDest, size_t iCount);

// Our protocol
	virtual void Imp_ReadInc(bool iIsFirst, ZRef<YadR>& oYadR) = 0;

private:
	ZRef<YadR> pReadInc();

	bool fStarted;
	ZRef<YadR> fValue;
	};

// =================================================================================================
#pragma mark -
#pragma mark ChanR_NameRefYad_Std

class ChanR_NameRefYad_Std
:	public ChanR_NameRefYad
	{
public:
	ChanR_NameRefYad_Std();
	ChanR_NameRefYad_Std(bool iFinished);

// From YadMapR
	virtual size_t Read(NameRefYad* oDest, size_t iCount);

// Our protocol
	virtual void Imp_ReadInc(bool iIsFirst, Name& oName, ZRef<YadR>& oYadR) = 0;

private:
	ZRef<YadR> pReadInc(Name& oName);

	bool fStarted;
	ZRef<YadR> fValue;
	};

} // namespace ZooLib

#endif // __ZooLib_Yad_Std_h__
