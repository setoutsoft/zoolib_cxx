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

#include "zoolib/ZUnicode_Normalize_Win.h"

#if 0 && ZCONFIG_SPI_Enabled(Win)

#include "zoolib/ZFunctionChain.h"
#include "zoolib/ZWinHeader.h"

NAMESPACE_ZOOLIB_BEGIN
namesapce ZUnicode {

// =================================================================================================
#pragma mark -
#pragma mark * ZUnicode, normalization, Win

namespace ZANONYMOUS {

static string16 sNormalized_C_Win(const string16& iString, ENormForm iNormForm)
	{
	// See FoldString with these flags:
	// FormC      MAP_PRECOMPOSED
	// FormD      MAP_COMPOSITE
	// FormKC     MAP_PRECOMPOSED | MAP_FOLDCZONE
	// FormKD     MAP_COMPOSITE | MAP_FOLDCZONE 
	// That said, FoldString uses old tables which do not match the
	// unicode standard behavior.

	// Vista has NormalizeString, which is what we should really use.
	}

class Function
:	public ZFunctionChain_T<string16, const Param_Normalize&>
	{
	virtual bool Invoke(Result_t& oResult, Param_t iParam)
		{
		oResult = sNormalized_C_Win(iParam.fString, iParam.fNormForm);
		return true;		
		}	
	} sFunction0;

} // namespace ZANONYMOUS

} // namespace ZUnicode
NAMESPACE_ZOOLIB_END

#endif // ZCONFIG_SPI_Enabled(ICU)
