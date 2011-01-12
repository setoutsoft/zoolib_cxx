/* -------------------------------------------------------------------------------------------------
Copyright (c) 2011 Andrew Green
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

#include "zoolib/ZVisitor_Expr_Bool_ValPred_Do_GetNames.h"
#include "zoolib/ZUtil_STL_set.h"
#include "zoolib/ZValPred_GetNames.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZVisitor_Expr_Bool_ValPred_Do_GetNames

void ZVisitor_Expr_Bool_ValPred_Do_GetNames::Visit_Expr_Bool_ValPred(
	const ZRef<ZExpr_Bool_ValPred>& iExpr)
	{ this->pSetResult(sGetNames(iExpr->GetValPred())); }

void ZVisitor_Expr_Bool_ValPred_Do_GetNames::Visit_Expr_Op1(
	const ZRef<ZExpr_Op1_T<ZExpr_Bool> >& iExpr)
	{ this->pSetResult(this->Do(iExpr->GetOp0())); }

void ZVisitor_Expr_Bool_ValPred_Do_GetNames::Visit_Expr_Op2(
	const ZRef<ZExpr_Op2_T<ZExpr_Bool> >& iExpr)
	{ this->pSetResult(ZUtil_STL::sOr(this->Do(iExpr->GetOp0()), this->Do(iExpr->GetOp1()))); }

std::set<std::string> sGetNames(const ZRef<ZExpr_Bool>& iExpr)
	{ return ZVisitor_Expr_Bool_ValPred_Do_GetNames().Do(iExpr); }

} // namespace ZooLib