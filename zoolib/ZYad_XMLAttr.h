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

#ifndef __ZYad_XMLAttr__
#define __ZYad_XMLAttr__ 1
#include "zconfig.h"

#include "zoolib/ZML.h"
#include "zoolib/ZYad.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZYadParseException_XMLAttr

class ZYadParseException_XMLAttr : public ZYadParseException
	{
public:
	ZYadParseException_XMLAttr(const std::string& iWhat);
	ZYadParseException_XMLAttr(const char* iWhat);
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZYad_XMLAttr

namespace ZYad_XMLAttr {

ZRef<ZYadR> sMakeYadR(ZRef<ZML::StrimmerU> iStrimmerU);

//void sToStrim(ZRef<ZYadR> iYadR, const ZML::StrimW& s);

} // namespace ZYad_XMLAttr
} // namespace ZooLib

#endif // __ZYad_XMLAttr__
