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

#include "zoolib/ZMACRO_foreach.h"
#include "zoolib/ZUtil_Strim_Operators.h"
#include "zoolib/ZYad_ZooLibStrim.h"

#include "zoolib/RelationalAlgebra/Util_Strim_RelHead.h"

namespace ZooLib {
namespace RelationalAlgebra {
namespace Util_Strim_RelHead {

// =================================================================================================
// MARK: - Util_Strim_RelHead

using std::set;

void sWrite_PropName(const string8& iName, const ZStrimW& s)
	{
	s.Write("@");
	ZYad_ZooLibStrim::sWrite_PropName(iName, s);
	}

void sWrite_RelHead(const RelHead& iRelHead, const ZStrimW& s)
	{
	s.Write("[");

	bool isFirst = true;
	foreachi (ii, iRelHead)
		{
		if (not sGetSet(isFirst, false))
			s.Write(", ");
		sWrite_PropName(*ii, s);
		}
	s.Write("]");
	}

} // namespace Util_Strim_RelHead
} // namespace RelationalAlgebra

const ZStrimW& operator<<(const ZStrimW& w, const RelationalAlgebra::RelHead& iRH)
	{
	RelationalAlgebra::Util_Strim_RelHead::sWrite_RelHead(iRH, w);
	return w;
	}

const ZStrimW& operator<<(const ZStrimW& w, const RelationalAlgebra::Rename& iRename)
	{
	w << "[";
	bool isFirst = true;
	foreachi (ii, iRename)
		{
		if (not sGetSet(isFirst, false))
			w << ", ";
		w << ii->second << "<--" << ii->first;
		}
	w << "]";
	return w;
	}

} // namespace ZooLib