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

#include "zoolib/Compare_Integer.h" // for sCompare_T<bool>
#include "zoolib/Compare_string.h"
#include "zoolib/ZMACRO_foreach.h"

#include "zoolib/RelationalAlgebra/RelHead.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark sCompare

template <>
int sCompare_T(const RelationalAlgebra::RelHead& iL, const RelationalAlgebra::RelHead& iR)
	{ return sCompareIterators_T(iL.begin(), iL.end(), iR.begin(), iR.end()); }

ZMACRO_CompareRegistration_T(RelationalAlgebra::RelHead)

template <>
int sCompare_T(const RelationalAlgebra::Rename& iL, const RelationalAlgebra::Rename& iR)
	{ return sCompareIterators_T(iL.begin(), iL.end(), iR.begin(), iR.end()); }

ZMACRO_CompareRegistration_T(RelationalAlgebra::Rename)

template <>
int sCompare_T(const RelationalAlgebra::ConcreteHead& iL, const RelationalAlgebra::ConcreteHead& iR)
	{ return sCompareIterators_T(iL.begin(), iL.end(), iR.begin(), iR.end()); }

ZMACRO_CompareRegistration_T(RelationalAlgebra::ConcreteHead)

} // namespace ZooLib

namespace ZooLib {
namespace RelationalAlgebra {

// =================================================================================================
#pragma mark -
#pragma mark Rename

Rename sInverted(const Rename& iRename)
	{
	Rename result;
	foreachi (ii, iRename)
		result[ii->second] = ii->first;
	return result;
	}

// =================================================================================================
#pragma mark -
#pragma mark sRelHead

RelHead sRelHead(const ColName& i0, const ColName& i1)
	{
	RelHead result;
	result.insert(i0);
	result.insert(i1);
	return result;
	}

RelHead sRelHead(const ColName& i0, const ColName& i1, const ColName& i2)
	{
	RelHead result;
	result.insert(i0);
	result.insert(i1);
	result.insert(i2);
	return result;
	}

RelHead sRelHead(const ColName& i0, const ColName& i1, const ColName& i2, const ColName& i3)
	{
	RelHead result;
	result.insert(i0);
	result.insert(i1);
	result.insert(i2);
	result.insert(i3);
	return result;
	}

RelHead sRelHead(const ColName& i0, const ColName& i1, const ColName& i2, const ColName& i3,
	const ColName& i4)
	{
	RelHead result;
	result.insert(i0);
	result.insert(i1);
	result.insert(i2);
	result.insert(i3);
	result.insert(i4);
	return result;
	}

RelHead sRelHead(
	const ColName& i0, const ColName& i1, const ColName& i2, const ColName& i3,
	const ColName& i4, const ColName& i5)
	{
	RelHead result;
	result.insert(i0);
	result.insert(i1);
	result.insert(i2);
	result.insert(i3);
	result.insert(i4);
	result.insert(i5);
	return result;
	}

RelHead sRelHead(
	const ColName& i0, const ColName& i1, const ColName& i2, const ColName& i3,
	const ColName& i4, const ColName& i5, const ColName& i6)
	{
	RelHead result;
	result.insert(i0);
	result.insert(i1);
	result.insert(i2);
	result.insert(i3);
	result.insert(i4);
	result.insert(i5);
	result.insert(i6);
	return result;
	}

RelHead sRelHead(
	const ColName& i0, const ColName& i1, const ColName& i2, const ColName& i3,
	const ColName& i4, const ColName& i5, const ColName& i6, const ColName& i7)
	{
	RelHead result;
	result.insert(i0);
	result.insert(i1);
	result.insert(i2);
	result.insert(i3);
	result.insert(i4);
	result.insert(i5);
	result.insert(i6);
	result.insert(i7);
	return result;
	}

// =================================================================================================
#pragma mark -
#pragma mark Prefix

bool sHasPrefix(const ColName& iPrefix, const ColName& iColName)
	{ return iColName.substr(0, iPrefix.size()) == iPrefix; }

ColName sPrefixInserted(const ColName& iPrefix, const ColName& iColName)
	{ return iPrefix + iColName; }

ColName sPrefixErased(const ColName& iPrefix, const ColName& iColName)
	{
	if (iColName.substr(0, iPrefix.size()) == iPrefix)
		return iColName.substr(iPrefix.size(), ColName::npos);
	return iColName;
	}

ZQ<ColName> sQPrefixErased(const ColName& iPrefix, const ColName& iColName)
	{
	if (iColName.substr(0, iPrefix.size()) == iPrefix)
		return iColName.substr(iPrefix.size(), ColName::npos);
	return null;
	}

ColName sRenamed(const Rename& iRename, const ColName& iColName)
	{
	Rename::const_iterator iter = iRename.find(iColName);
	if (iRename.end() == iter)
		return iColName;
	return iter->second;
	}

bool sHasPrefix(const ColName& iPrefix, const RelHead& iRelHead)
	{
	if (iPrefix.empty())
		return true;

	foreachi (ii, iRelHead)
		{
		if (not sHasPrefix(iPrefix, *ii))
			return false;
		}

	return true;
	}

RelHead sPrefixInserted(const ColName& iPrefix, const RelHead& iRelHead)
	{
	if (iPrefix.empty())
		return iRelHead;

	RelHead result;
	foreachi (ii, iRelHead)
		result.insert(sPrefixInserted(iPrefix, *ii));

	return result;
	}

RelHead sPrefixErased(const ColName& iPrefix, const RelHead& iRelHead)
	{
	if (iPrefix.empty())
		return iRelHead;

	RelHead result;
	foreachi (ii, iRelHead)
		result.insert(sPrefixErased(iPrefix, *ii));

	return result;
	}

ZQ<RelHead> sQPrefixErased(const ColName& iPrefix, const RelHead& iRelHead)
	{
	if (iPrefix.empty())
		return iRelHead;

	RelHead result;
	foreachi (ii, iRelHead)
		{
		if (ZQ<ColName> theQ = sQPrefixErased(iPrefix, *ii))
			result.insert(*theQ);
		else
			return null;
		}

	return result;
	}

RelHead sRenamed(const Rename& iRename, const RelHead& iRelHead)
	{
	if (iRename.empty())
		return iRelHead;

	RelHead result;
	foreachi (ii, iRelHead)
		result.insert(sRenamed(iRename, *ii));

	return result;
	}

RelHead sNamesFrom(const Rename& iRename)
	{
	RelHead result;
	foreachi (ii, iRename)
		result.insert(ii->first);
	return result;
	}

RelHead sNamesTo(const Rename& iRename)
	{
	RelHead result;
	foreachi (ii, iRename)
		result.insert(ii->second);
	return result;
	}

// =================================================================================================
#pragma mark -
#pragma mark ConcreteHead

ConcreteHead sConcreteHead(const RelHead& iRequired)
	{
	ConcreteHead result;
	for (RelHead::const_iterator ii = iRequired.begin(), end = iRequired.end();
		ii != end; ++ii)
		{ result[*ii] = true; }
	return result;
	}

ConcreteHead sConcreteHead(const RelHead& iRequired, const RelHead& iOptional)
	{
	ConcreteHead result;

	for (RelHead::const_iterator ii = iOptional.begin(), end = iOptional.end();
		ii != end; ++ii)
		{ result[*ii] = false; }

	for (RelHead::const_iterator ii = iRequired.begin(), end = iRequired.end();
		ii != end; ++ii)
		{ result[*ii] = true; }

	return result;
	}

ConcreteHead sAugmentedOptional(const ConcreteHead& iConcreteHead, const RelHead& iOptional)
	{
	ConcreteHead result = iConcreteHead;

	for (RelHead::const_iterator ii = iOptional.begin(), end = iOptional.end();
		ii != end; ++ii)
		{
		// Evaluating this will get a false value into a non-extant slot, and and otherwise
		// have no effect.
		result[*ii];
		}

	return result;
	}

ConcreteHead sAugmentedRequired(const ConcreteHead& iConcreteHead, const RelHead& iRequired)
	{
	ConcreteHead result = iConcreteHead;

	for (RelHead::const_iterator ii = iRequired.begin(), end = iRequired.end();
		ii != end; ++ii)
		{ result[*ii] = true; }

	return result;
	}


RelHead sRelHead_Required(const ConcreteHead& iConcreteHead)
	{
	RelHead result;
	for (ConcreteHead::const_iterator ii = iConcreteHead.begin(), end = iConcreteHead.end();
		ii != end; ++ii)
		{
		if (ii->second)
			Util_STL::sInsert(result, ii->first);
		}
	return result;
	}

RelHead sRelHead_Optional(const ConcreteHead& iConcreteHead)
	{
	RelHead result;
	for (ConcreteHead::const_iterator ii = iConcreteHead.begin(), end = iConcreteHead.end();
		ii != end; ++ii)
		{
		if (not ii->second)
			Util_STL::sInsert(result, ii->first);
		}
	return result;
	}

RelHead sRelHead(const ConcreteHead& iConcreteHead)
	{
	RelHead result;
	for (ConcreteHead::const_iterator ii = iConcreteHead.begin(), end = iConcreteHead.end();
		ii != end; ++ii)
		{ Util_STL::sInsert(result, ii->first); }
	return result;
	}

void sRelHeads(const ConcreteHead& iConcreteHead, RelHead& oRequired, RelHead& oOptional)
	{
	for (ConcreteHead::const_iterator ii = iConcreteHead.begin(), end = iConcreteHead.end();
		ii != end; ++ii)
		{
		if (ii->second)
			Util_STL::sInsert(oRequired, ii->first);
		else
			Util_STL::sInsert(oOptional, ii->first);
		}
	}

} // namespace RelationalAlgebra
} // namespace ZooLib
