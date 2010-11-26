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

#ifndef __ZVisitor_Expr_Bool_DoEval__
#define __ZVisitor_Expr_Bool_DoEval__
#include "zconfig.h"

#include "zoolib/ZExpr_Bool.h"
#include "zoolib/ZVisitor_Do_T.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZVisitor_Expr_Bool_DoEval

class ZVisitor_Expr_Bool_DoEval
:	public virtual ZVisitor_Do_T<bool>
,	public virtual ZVisitor_Expr_Bool_True
,	public virtual ZVisitor_Expr_Bool_False
,	public virtual ZVisitor_Expr_Bool_Not
,	public virtual ZVisitor_Expr_Bool_And
,	public virtual ZVisitor_Expr_Bool_Or
	{
public:
// From ZVisitor_Expr_Bool_XXX
	virtual void Visit_Expr_Bool_True(ZRef<ZExpr_Bool_True> iRep);
	virtual void Visit_Expr_Bool_False(ZRef<ZExpr_Bool_False> iRep);
	virtual void Visit_Expr_Bool_Not(ZRef<ZExpr_Bool_Not> iRep);
	virtual void Visit_Expr_Bool_And(ZRef<ZExpr_Bool_And> iRep);
	virtual void Visit_Expr_Bool_Or(ZRef<ZExpr_Bool_Or> iRep);
	};

} // namespace ZooLib

#endif // __ZVisitor_Expr_Bool_DoEval__