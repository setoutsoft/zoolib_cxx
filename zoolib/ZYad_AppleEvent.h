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

#ifndef __ZYad_AppleEvent_h__
#define __ZYad_AppleEvent_h__ 1
#include "zconfig.h"
#include "zoolib/ZCONFIG_SPI.h"

#if ZCONFIG_SPI_Enabled(AppleEvent)

#include "zoolib/ZVal_AppleEvent.h"
#include "zoolib/ZYad_Val_T.h"

namespace ZooLib {

typedef ZYadR_Val_T<ZVal_AppleEvent> ZYadR_AppleEvent;
typedef ZYadSatRPos_Val_T<ZSeq_AppleEvent> ZYadSatRPos_AppleEvent;
typedef ZYadMatRPos_Val_T<ZMap_AppleEvent> ZYadMatRPos_AppleEvent;

// =================================================================================================
// MARK: - sYadR

ZRef<ZYadR> sYadR(const ZVal_AppleEvent& iVal);

ZRef<ZYadSatRPos> sYadR(const ZSeq_AppleEvent& iSeq);

ZRef<ZYadMatRPos> sYadR(const ZMap_AppleEvent& iMap);

} // namespace ZooLib

#endif // ZCONFIG_SPI_Enabled(AppleEvent)

#endif // __ZYad_AppleEvent_h__
