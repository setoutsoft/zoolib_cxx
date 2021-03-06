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

#include "zoolib/RelationalAlgebra/Util_Rel_Operators.h"

namespace ZooLib {
namespace RelationalAlgebra {

ZRef<Expr_Rel> sConst(const NameVal& iNameVal)
	{ return sConst(iNameVal.first, iNameVal.second); }

ZRef<Expr_Rel> operator*(const NameVal& iNameVal, const ZRef<Expr_Rel>& iRel)
	{ return sConst(iNameVal) * iRel; }

ZRef<Expr_Rel> operator*(const ZRef<Expr_Rel>& iRel, const NameVal& iNameVal)
	{ return iRel * sConst(iNameVal); }

ZRef<Expr_Rel>& operator*=(ZRef<Expr_Rel>& ioRel, const NameVal& iNameVal)
	{ return ioRel *= sConst(iNameVal); }

// -----

static ZRef<Expr_Rel> spConst(const Map_Any& iMap)
	{
	ZRef<Expr_Rel> result;
	for (Map_Any::Index_t i = iMap.Begin(); i != iMap.End(); ++i)
		{
		ZRef<Expr_Rel> cur = sConst(iMap.NameOf(i), iMap.Get(i));
		if (not result)
			result = cur;
		else
			result = result * cur;
		}
	return result;
	}

ZRef<Expr_Rel> sConst(const Map_Any& iMap)
	{
	if (ZRef<Expr_Rel> result = spConst(iMap))
		return result;

	return sDee();
	}

ZRef<Expr_Rel> operator*(const Map_Any& iMap, const ZRef<Expr_Rel>& iRel)
	{
	if (ZRef<Expr_Rel> asRel = spConst(iMap))
		return asRel * iRel;
	return iRel;
	}

ZRef<Expr_Rel> operator*(const ZRef<Expr_Rel>& iRel, const Map_Any& iMap)
	{
	if (ZRef<Expr_Rel> asRel = spConst(iMap))
		return iRel * asRel;
	return iRel;
	}

ZRef<Expr_Rel>& operator*=(ZRef<Expr_Rel>& ioRel, const Map_Any& iMap)
	{
	if (ZRef<Expr_Rel> asRel = spConst(iMap))
		ioRel = ioRel * asRel;
	return ioRel;
	}

// -----

ZRef<Expr_Rel> operator&(const ZRef<Expr_Rel>& iExpr_Rel, const ValPred& iValPred)
	{ return new Expr_Rel_Restrict(iExpr_Rel, new Expr_Bool_ValPred(iValPred)); }

ZRef<Expr_Rel> operator&(const ValPred& iValPred, const ZRef<Expr_Rel>& iExpr_Rel)
	{ return new Expr_Rel_Restrict(iExpr_Rel, new Expr_Bool_ValPred(iValPred)); }

ZRef<Expr_Rel>& operator&=(ZRef<Expr_Rel>& ioExpr_Rel, const ValPred& iValPred)
	{ return ioExpr_Rel = ioExpr_Rel & iValPred; }

} // namespace RelationalAlgebra
} // namespace ZooLib
