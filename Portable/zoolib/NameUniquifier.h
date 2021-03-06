/* -------------------------------------------------------------------------------------------------
Copyright (c) 2012 Andrew Green
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

#ifndef __ZooLib_NameUniquifier_h__
#define __ZooLib_NameUniquifier_h__ 1
#include "zconfig.h"

#include "zoolib/Name.h"
#include "zoolib/ThreadVal.h"
#include "zoolib/Uniquifier.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark CountedStringUniquifier

typedef CountedVal<string8> CountedString;

typedef ZRef<CountedString> ZRefCountedString;

struct Compare_RefCountedString
	{ bool operator()(const ZRefCountedString& l, const ZRefCountedString& r) const; };

typedef Uniquifier<ZRefCountedString,Compare_RefCountedString> CountedStringUniquifier;

// =================================================================================================
#pragma mark -
#pragma mark ThreadVal_NameUniquifier

typedef ThreadVal<CountedStringUniquifier, struct Tag_NameUniquifier> ThreadVal_NameUniquifier;

// =================================================================================================
#pragma mark -
#pragma mark sName

inline Name sName(const char* iConstCharStar)
	{ return Name(iConstCharStar); }

Name sName(const string8& iString);

Name sName(const ZRefCountedString& iCountedString);

} // namespace ZooLib

#endif // __ZooLib_NameUniquifier_h__
