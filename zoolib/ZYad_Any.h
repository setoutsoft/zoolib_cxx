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

#ifndef __ZYad_Any__
#define __ZYad_Any__ 1
#include "zconfig.h"

#include "zoolib/ZVal_Any.h"
#include "zoolib/ZYad_Val_T.h"

NAMESPACE_ZOOLIB_BEGIN

typedef ZYadR_Val_T<ZVal_Any> ZYadR_Any;
typedef ZYadStreamRPos_Val_T<ZValData_Any> ZYadStreamRPos_Any;
typedef ZYadListRPos_Val_T<ZValList_Any> ZYadListRPos_Any;
typedef ZYadMapRPos_Val_T<ZValMap_Any> ZYadMapRPos_Any;

// =================================================================================================
#pragma mark -
#pragma mark * sMakeYadR

ZRef<ZYadR> sMakeYadR(const ZVal_Any& iVal);

ZRef<ZYadStreamR> sMakeYadR(const ZValData_Any& iData);

ZRef<ZYadListR> sMakeYadR(const ZValList_Any& iList);

ZRef<ZYadMapR> sMakeYadR(const ZValMap_Any& iMap);

NAMESPACE_ZOOLIB_END

#endif // __ZYad_Any__
