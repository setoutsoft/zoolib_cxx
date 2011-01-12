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

#include "zoolib/ZCompare_Ref.h"
#include "zoolib/zra/ZRA_Expr_Rel_Restrict.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * sCompare_T

template <>
int sCompare_T(const ZRA::Expr_Rel_Restrict& iL, const ZRA::Expr_Rel_Restrict& iR)
	{
	if (int compare = sCompare_T(iL.GetValPred(), iR.GetValPred()))
		return compare;

	return sCompare_T(iL.GetOp0(), iR.GetOp0());
	}

ZMACRO_CompareRegistration_T(ZRA::Expr_Rel_Restrict)

namespace ZRA {

// =================================================================================================
#pragma mark -
#pragma mark * Expr_Rel_Restrict

Expr_Rel_Restrict::Expr_Rel_Restrict(
	const ZRef<Expr_Rel>& iOp0, const ZValPred& iValPred)
:	inherited(iOp0)
,	fValPred(iValPred)
	{}

Expr_Rel_Restrict::~Expr_Rel_Restrict()
	{}

void Expr_Rel_Restrict::Accept(ZVisitor& iVisitor)
	{
	if (Visitor_Expr_Rel_Restrict* theVisitor = dynamic_cast<Visitor_Expr_Rel_Restrict*>(&iVisitor))
		this->Accept_Expr_Rel_Restrict(*theVisitor);
	else
		inherited::Accept(iVisitor);
	}

void Expr_Rel_Restrict::Accept_Expr_Op1(ZVisitor_Expr_Op1_T<Expr_Rel>& iVisitor)
	{
	if (Visitor_Expr_Rel_Restrict* theVisitor = dynamic_cast<Visitor_Expr_Rel_Restrict*>(&iVisitor))
		this->Accept_Expr_Rel_Restrict(*theVisitor);
	else
		inherited::Accept_Expr_Op1(iVisitor);
	}

ZRef<Expr_Rel> Expr_Rel_Restrict::Self()
	{ return this; }

ZRef<Expr_Rel> Expr_Rel_Restrict::Clone(const ZRef<Expr_Rel>& iOp0)
	{ return new Expr_Rel_Restrict(iOp0, fValPred); }

void Expr_Rel_Restrict::Accept_Expr_Rel_Restrict(Visitor_Expr_Rel_Restrict& iVisitor)
	{ iVisitor.Visit_Expr_Rel_Restrict(this); }

const ZValPred& Expr_Rel_Restrict::GetValPred() const
	{ return fValPred; }

// =================================================================================================
#pragma mark -
#pragma mark * Visitor_Expr_Rel_Restrict

void Visitor_Expr_Rel_Restrict::Visit_Expr_Rel_Restrict(const ZRef<Expr_Rel_Restrict >& iExpr)
	{ this->Visit_Expr_Op1(iExpr); }

// =================================================================================================
#pragma mark -
#pragma mark * Operators

ZRef<Expr_Rel_Restrict> sRestrict(
	const ZValPred& iValPred, const ZRef<Expr_Rel>& iExpr_Rel)
	{ return new Expr_Rel_Restrict(iExpr_Rel, iValPred); }

ZRef<Expr_Rel> operator&(const ZRef<Expr_Rel>& iExpr_Rel, const ZValPred& iValPred)
	{ return sRestrict(iValPred, iExpr_Rel); }

ZRef<Expr_Rel> operator&(const ZValPred& iValPred, const ZRef<Expr_Rel>& iExpr_Rel)
	{ return sRestrict(iValPred, iExpr_Rel); }

ZRef<Expr_Rel>& operator&=(ZRef<Expr_Rel>& ioExpr_Rel, const ZValPred& iValPred)
	{ return ioExpr_Rel = ioExpr_Rel & iValPred; }

} // namespace ZRA
} // namespace ZooLib