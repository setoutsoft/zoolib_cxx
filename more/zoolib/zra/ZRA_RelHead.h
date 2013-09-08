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

#ifndef __ZRA_RelHead_h__
#define __ZRA_RelHead_h__ 1
#include "zconfig.h"

#include "zoolib/ZUtil_STL_set.h"

#include "zoolib/zra/ZRA_ColName.h"

#include <map>
#include <set>

namespace ZooLib {
namespace ZRA {

// =================================================================================================
// MARK: - Rename

typedef std::map<ColName,ColName> Rename;

Rename sInverted(const Rename& iRename);

// =================================================================================================
// MARK: - RelHead

typedef std::set<ColName> RelHead;

inline RelHead sRelHead(const ColName* iElem, size_t iCount)
	{ return RelHead(iElem, iElem + iCount); }

inline RelHead sRelHead(const ColName& iElem)
	{ return sRelHead(&iElem, 1); }

inline RelHead sRelHead(const char* iElem)
	{ return sRelHead(ColName(iElem)); }

inline size_t sIndexOf(const RelHead& iRelHead, const ColName& iElem)
	{ return std::distance(iRelHead.begin(), iRelHead.find(iElem)); }

// =================================================================================================
// MARK: - RelHead operators

// Incorporate set operators from ZUtil_STL

using ZUtil_STL::operator&;
using ZUtil_STL::operator&=;

using ZUtil_STL::operator|;
using ZUtil_STL::operator|=;

using ZUtil_STL::operator^;
using ZUtil_STL::operator^=;

using ZUtil_STL::operator-;
using ZUtil_STL::operator-=;

inline RelHead operator&(const RelHead& iRelHead, const char* iElem)
	{ return iRelHead & RelHead::key_type(iElem); }

inline RelHead& operator&=(RelHead& ioRelHead, const char* iElem)
	{ return ioRelHead &= RelHead::key_type(iElem); }

inline RelHead operator&(const char* iElem, const RelHead& iRelHead)
	{ return iRelHead & RelHead::key_type(iElem); }

inline RelHead operator|(const RelHead& iRelHead, const char* iElem)
	{ return iRelHead | RelHead::key_type(iElem); }

inline RelHead& operator|=(RelHead& ioRelHead, const char* iElem)
	{ return ioRelHead |= RelHead::key_type(iElem); }

inline RelHead operator|(const char* iElem, const RelHead& iRelHead)
	{ return iRelHead | RelHead::key_type(iElem); }

inline RelHead operator^(const RelHead& iRelHead, const char* iElem)
	{ return iRelHead ^ RelHead::key_type(iElem); }

inline RelHead& operator^=(RelHead& ioRelHead, const char* iElem)
	{ return ioRelHead ^= RelHead::key_type(iElem); }

inline RelHead operator^(const char* iElem, const RelHead& iRelHead)
	{ return iRelHead ^ RelHead::key_type(iElem); }

inline RelHead operator-(const RelHead& iRelHead, const char* iElem)
	{ return iRelHead - RelHead::key_type(iElem); }

inline RelHead& operator-=(RelHead& ioRelHead, const char* iElem)
	{ return ioRelHead -= RelHead::key_type(iElem); }

// =================================================================================================
// MARK: - Prefix

bool sHasPrefix(const ColName& iPrefix, const ColName& iColName);
ColName sPrefixInserted(const ColName& iPrefix, const ColName& iColName);
ColName sPrefixErased(const ColName& iPrefix, const ColName& iColName);
ColName sRenamed(const Rename& iRename, const ColName& iColName);

bool sHasPrefix(const ColName& iPrefix, const RelHead& iRelHead);
RelHead sPrefixInserted(const ColName& iPrefix, const RelHead& iRelHead);
RelHead sPrefixErased(const ColName& iPrefix, const RelHead& iRelHead);
RelHead sRenamed(const Rename& iRename, const RelHead& iRelHead);

} // namespace ZRA
} // namespace ZooLib

#endif // __ZRA_RelHead_h__
