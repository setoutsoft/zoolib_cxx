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

#include "zoolib/Compare_Ref.h"
#include "zoolib/Compare_string.h"
#include "zoolib/RelationalAlgebra/Expr_Rel_Const.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark sCompare_T

template <>
int sCompare_T(const RelationalAlgebra::Expr_Rel_Const& iL,
	const RelationalAlgebra::Expr_Rel_Const& iR)
	{
	if (int compare = sCompare_T(iL.GetColName(), iR.GetColName()))
		return compare;

	return sCompare_T(iL.GetVal(), iR.GetVal());
	}

ZMACRO_CompareRegistration_T(RelationalAlgebra::Expr_Rel_Const)

namespace RelationalAlgebra {

// =================================================================================================
#pragma mark -
#pragma mark Expr_Rel_Const

Expr_Rel_Const::Expr_Rel_Const(const ColName& iColName, const Val_Any& iVal)
:	fColName(iColName)
,	fVal(iVal)
	{}

Expr_Rel_Const::~Expr_Rel_Const()
	{}

void Expr_Rel_Const::Accept(const Visitor& iVisitor)
	{
	if (Visitor_Expr_Rel_Const* theVisitor = sDynNonConst<Visitor_Expr_Rel_Const>(&iVisitor))
		this->Accept_Expr_Rel_Const(*theVisitor);
	else
		inherited::Accept(iVisitor);
	}

void Expr_Rel_Const::Accept_Expr_Op0(Visitor_Expr_Op0_T<Expr_Rel>& iVisitor)
	{
	if (Visitor_Expr_Rel_Const* theVisitor = sDynNonConst<Visitor_Expr_Rel_Const>(&iVisitor))
		this->Accept_Expr_Rel_Const(*theVisitor);
	else
		inherited::Accept_Expr_Op0(iVisitor);
	}

ZRef<Expr_Rel> Expr_Rel_Const::Self()
	{ return this; }

ZRef<Expr_Rel> Expr_Rel_Const::Clone()
	{ return this; }

void Expr_Rel_Const::Accept_Expr_Rel_Const(Visitor_Expr_Rel_Const& iVisitor)
	{ iVisitor.Visit_Expr_Rel_Const(this); }

const ColName& Expr_Rel_Const::GetColName() const
	{ return fColName; }

const Val_Any& Expr_Rel_Const::GetVal() const
	{ return fVal; }

// =================================================================================================
#pragma mark -
#pragma mark Visitor_Expr_Rel_Const

void Visitor_Expr_Rel_Const::Visit_Expr_Rel_Const(const ZRef<Expr_Rel_Const>& iExpr)
	{ this->Visit_Expr_Op0(iExpr); }

// =================================================================================================
#pragma mark -
#pragma mark Relational operators

ZRef<Expr_Rel> sConst(const ColName& iColName, const Val_Any& iVal)
	{ return new Expr_Rel_Const(iColName, iVal); }

} // namespace RelationalAlgebra
} // namespace ZooLib
