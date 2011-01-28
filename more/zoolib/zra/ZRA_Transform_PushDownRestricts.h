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

#ifndef __ZRA_Transform_PushDownRestricts__
#define __ZRA_Transform_PushDownRestricts__
#include "zconfig.h"

#include "zoolib/ZVisitor_Expr_Op_Do_Transform_T.h"
#include "zoolib/zra/ZRA_Expr_Rel_Calc.h"
#include "zoolib/zra/ZRA_Expr_Rel_Concrete.h"
#include "zoolib/zra/ZRA_Expr_Rel_Const.h"
#include "zoolib/zra/ZRA_Expr_Rel_Embed.h"
#include "zoolib/zra/ZRA_Expr_Rel_Rename.h"
#include "zoolib/zra/ZRA_Expr_Rel_Restrict.h"

#include <vector>

namespace ZooLib {
namespace ZRA {

// =================================================================================================
#pragma mark -
#pragma mark * ZRA::Transform_PushDownRestricts

class Transform_PushDownRestricts
:	public virtual ZVisitor_Expr_Op_Do_Transform_T<Expr_Rel>
,	public virtual Visitor_Expr_Rel_Rename
,	public virtual Visitor_Expr_Rel_Restrict
,	public virtual Visitor_Expr_Rel_Embed
,	public virtual Visitor_Expr_Rel_Calc
,	public virtual Visitor_Expr_Rel_Concrete
,	public virtual Visitor_Expr_Rel_Const
	{
public:
// From Visitor_Expr_Rel_XXX
	virtual void Visit_Expr_Rel_Embed(const ZRef<Expr_Rel_Embed>& iExpr);
	virtual void Visit_Expr_Rel_Rename(const ZRef<Expr_Rel_Rename>& iExpr);
	virtual void Visit_Expr_Rel_Restrict(const ZRef<Expr_Rel_Restrict>& iExpr);

	virtual void Visit_Expr_Rel_Calc(const ZRef<Expr_Rel_Calc>& iExpr);
	virtual void Visit_Expr_Rel_Concrete(const ZRef<Expr_Rel_Concrete>& iExpr);
	virtual void Visit_Expr_Rel_Const(const ZRef<Expr_Rel_Const>& iExpr);

protected:
	void pHandleIt(const RelHead& iRH, const ZRef<Expr_Rel>& iRel);

	struct Restrict
		{
		ZRef<ZExpr_Bool> fExpr_Bool;
		size_t fCountTouching;
		size_t fCountSubsuming;
		};
	std::vector<Restrict*> fRestricts;
	};

} // namespace ZRA
} // namespace ZooLib

#endif // __ZRA_Transform_PushDownRestricts__