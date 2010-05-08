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

#ifndef __ZRA_RelHead__
#define __ZRA_RelHead__ 1
#include "zconfig.h"

#include "zoolib/ZUniSet_T.h"

#include <string>
#include <map>

NAMESPACE_ZOOLIB_BEGIN
namespace ZRA {

// =================================================================================================
#pragma mark -
#pragma mark * RelHead

typedef ZUniSet_T<std::string> RelHead;

inline RelHead operator&(const RelHead& iRelHead, const char* iElem)
	{ return iRelHead & std::string(iElem); }

inline RelHead operator&(const char* iElem, const RelHead& iRelHead)
	{ return iRelHead & std::string(iElem); }

inline RelHead operator|(const char* iElem, const RelHead& iRelHead)
	{ return iRelHead | std::string(iElem); }

inline RelHead operator|(const RelHead& iRelHead, const char* iElem)
	{ return iRelHead | std::string(iElem); }

inline RelHead operator-(const RelHead& iRelHead, const char* iElem)
	{ return iRelHead - std::string(iElem); }

inline RelHead operator^(const char* iElem, const RelHead& iRelHead)
	{ return iRelHead ^ std::string(iElem); }

inline RelHead operator^(const RelHead& iRelHead, const char* iElem)
	{ return iRelHead ^ std::string(iElem); }

// =================================================================================================
#pragma mark -
#pragma mark * Rename_t

typedef std::map<std::string, std::string> Rename_t;

Rename_t sInvert(const Rename_t& iRename);

} // namespace ZRA
NAMESPACE_ZOOLIB_END

#endif // __ZRA_RelHead__
