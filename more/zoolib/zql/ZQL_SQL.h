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

#ifndef __ZQL_SQL__
#define __ZQL_SQL__
#include "zconfig.h"

#include "zoolib/ZUnicodeString.h"
#include "zoolib/zql/ZQL_Expr_Rel_Concrete.h"

#include <map>
#include <vector>

NAMESPACE_ZOOLIB_BEGIN
namespace ZQL {
namespace SQL {

// =================================================================================================
#pragma mark -
#pragma mark * ZQL::SQL::Expr_Rel_SFW

class Expr_Rel_SFW : public Expr_Rel_Concrete
	{
public:
	Expr_Rel_SFW(const std::map<string8, string8>& iRenameMap,
		const RelHead& iRelHead,
		ZRef<ZExpr_Logic> iCondition,
		const std::vector<ZRef<Expr_Rel_Concrete> >& iRels);

// From Expr_Rel_Concrete
	virtual RelHead GetRelHead();

// Our protocol
	const std::map<string8, string8>& GetRenameMap();
	ZRef<ZExpr_Logic> GetCondition();
	const std::vector<ZRef<Expr_Rel_Concrete> >& GetRels();

public:
	const std::map<string8, string8> fRenameMap;
	const RelHead fRelHead;
	const ZRef<ZExpr_Logic> fCondition;
	const std::vector<ZRef<Expr_Rel_Concrete> > fRels;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZQL::SQL::sConvert

ZRef<Expr_Rel_SFW> sConvert(ZRef<Expr_Rel> iExpr);

} // namespace SQL
} // namespace ZQL
NAMESPACE_ZOOLIB_END

#endif // __ZQL_SQL__
