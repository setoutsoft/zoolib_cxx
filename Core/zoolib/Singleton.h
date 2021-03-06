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

#ifndef __ZooLib_Singleton_h__
#define __ZooLib_Singleton_h__ 1
#include "zconfig.h"

#include "zoolib/Deleter.h"

#include "zoolib/ZAtomic.h"
#include "zoolib/ZDebug.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark sSingleton

template <class Type_p, class Tag_p>
Type_p& sSingleton()
	{
	static Type_p* spType_p;
	if (not spType_p)
		{
		Type_p* theType_p = new Type_p();
		// The parens after Type_p are *essential*. If Type_p is POD then *theType_p will still
		// be properly default initialized -- without the parens that does not happen.
		// And yes, I've seen it happen in real life.
		if (not sAtomicPtr_CAS(&spType_p, nullptr, theType_p))
			delete theType_p;
		else
			static Deleter<Type_p> deleter(spType_p);
		}
	ZAssert(spType_p);
	return *spType_p;
	}

template <class Type_p>
Type_p& sSingleton()
	{ return sSingleton<Type_p,Type_p>(); }

} // namespace ZooLib

#endif // __ZooLib_Singleton_h___
