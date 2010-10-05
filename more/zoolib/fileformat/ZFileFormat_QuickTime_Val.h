/* -------------------------------------------------------------------------------------------------
Copyright (c) 2010 Andrew Green
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

#ifndef __ZFileFormat_QuickTime_Val__
#define __ZFileFormat_QuickTime_Val__ 1
#include "zconfig.h"

#include "zoolib/ZTagged_T.h"
#include "zoolib/ZVal_Any.h"

namespace ZooLib {
namespace FileFormat {
namespace QuickTime {

// =================================================================================================
#pragma mark -
#pragma mark * sAsVal

typedef ZTagged_T<struct BMP, ZData_Any> Data_BMP;
typedef ZTagged_T<struct JPEG, ZData_Any> Data_JPEG;
typedef ZTagged_T<struct PNG, ZData_Any> Data_PNG;
typedef ZTagged_T<struct Meta, ZData_Any> Data_Meta;

ZQ<ZVal_Any> sAsVal(const void* iSource, size_t iSize);
ZQ<ZVal_Any> sAsVal(const ZData_Any& iData);
ZQ<ZVal_Any> sAsVal(const ZStreamR& iStreamR);


} // namespace QuickTime
} // namespace FileFormat
} // namespace ZooLib

#endif // __ZFileFormat_QuickTime_Val__