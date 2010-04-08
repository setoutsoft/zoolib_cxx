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

#include "zoolib/ZYad_Any.h"
#include "zoolib/valbase/ZValBase.h"
#include "zoolib/valbase/ZValBase_Any.h"
#include "zoolib/zqe/ZQE_Result_Any.h"
#include "zoolib/zql/ZQL_ExprRep_Relation_Concrete.h"

NAMESPACE_ZOOLIB_BEGIN
namespace ZValBase_Any {

// =================================================================================================
#pragma mark -
#pragma mark * Iterator

class Iterator : public ZQE::Iterator
	{
public:
	Iterator(const ZSeq_Any& iSeq);
	Iterator(const ZSeq_Any& iSeq, size_t iIndex);

	virtual ~Iterator();
	
	virtual ZRef<ZQE::Iterator> Clone();
	virtual ZRef<ZQE::Result> ReadInc();

private:
	const ZSeq_Any fSeq;
	size_t fIndex;
	};

Iterator::Iterator(const ZSeq_Any& iSeq)
:	fSeq(iSeq)
,	fIndex(0)
	{}

Iterator::Iterator(const ZSeq_Any& iSeq, size_t iIndex)
:	fSeq(iSeq)
,	fIndex(iIndex)
	{}

Iterator::~Iterator()
	{}

ZRef<ZQE::Iterator> Iterator::Clone()
	{ return new Iterator(fSeq, fIndex); }

ZRef<ZQE::Result> Iterator::ReadInc()
	{
	if (fIndex < fSeq.Count())
		return new ZQE::Result_Any(fSeq.Get(fIndex++));
	return ZRef<ZQE::Result>();
	}

// =================================================================================================
#pragma mark -
#pragma mark * ExprRep_Relation_Concrete

class ExprRep_Relation_Concrete : public ZValBase::ExprRep_Relation_Concrete
	{
public:
	ExprRep_Relation_Concrete(const ZSeq_Any& iSeq);

// From ZValBase::ExprRep_Relation_Concrete
	virtual ZRef<ZQE::Iterator> MakeIterator();

private:
	const ZSeq_Any fSeq;
	};

ExprRep_Relation_Concrete::ExprRep_Relation_Concrete(const ZSeq_Any& iSeq)
:	fSeq(iSeq)
	{}

ZRef<ZQE::Iterator> ExprRep_Relation_Concrete::MakeIterator()
	{ return new Iterator(fSeq); }

// =================================================================================================
#pragma mark -
#pragma mark * ZValBase_Any pseudo constructors

ZRef<ZQL::ExprRep_Relation> sConcrete(const ZSeq_Any& iSeq)
	{
	// Could do a dynamic cast on iYadSeqR to see if it's really a ZYadSeqRPos,
	// in which case returning a ZValBase_YadSeqRPos::Iterator would be a win.
	return new ExprRep_Relation_Concrete(iSeq);
	}

} // namespace ZValBase_YadSeqR
NAMESPACE_ZOOLIB_END
