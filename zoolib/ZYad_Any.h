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

#ifndef __ZYad_Any_h__
#define __ZYad_Any_h__ 1
#include "zconfig.h"

#include "zoolib/ZVal_Any.h"
#include "zoolib/ZYad_Val_T.h"

namespace ZooLib {

typedef ZYadStreamerRPos_Val_T<ZData_Any> ZYadStreamerRPos_Any;
typedef ZYadSeqAtRPos_Val_T<ZSeq_Any> ZYadSeqAtRPos_Any;
typedef ZYadMapAtRPos_Val_T<ZMap_Any> ZYadMapAtRPos_Any;

// =================================================================================================
// MARK: - sYadR

ZRef<ZYadR> sYadR(const ZAny& iVal);

ZRef<ZYadStreamerR> sYadR(const ZData_Any& iData);

ZRef<ZYadSeqAtRPos> sYadR(const ZSeq_Any& iSeq);

ZRef<ZYadMapAtRPos> sYadR(const ZMap_Any& iMap);

// =================================================================================================
// MARK: - sFromYadR

ZVal_Any sFromYadR(const ZVal_Any& iDefault, ZRef<ZYadR> iYadR);
ZVal_Any sFromYadR(bool iRepeatedPropsAsSeq, const ZVal_Any& iDefault, ZRef<ZYadR> iYadR);

} // namespace ZooLib

#endif // __ZYad_Any_h__
