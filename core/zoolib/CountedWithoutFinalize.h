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

#ifndef __ZooLib_CountedWithoutFinalize_h__
#define __ZooLib_CountedWithoutFinalize_h__ 1
#include "zconfig.h"

#include "zoolib/ThreadSafe.h"

namespace ZooLib {

// =================================================================================================
// MARK: - CountedWithoutFinalize

class CountedWithoutFinalize
	{
public:
	CountedWithoutFinalize();
	virtual ~CountedWithoutFinalize();

	void Retain() { ZMACRO_ThreadSafe_Inc(fRefCount); }
	void Release();
	bool IsShared() const;
	bool IsReferenced() const;

private:
	ThreadSafe_t fRefCount;
	};

// =================================================================================================
// MARK: - sRetain/sRelease for CountedWithoutFinalize derivatives

inline void sRetain(CountedWithoutFinalize& iObject)
	{ iObject.Retain(); }

inline void sRelease(CountedWithoutFinalize& iObject)
	{ iObject.Release(); }

inline void sCheck(CountedWithoutFinalize* iP)
	{}

} // namespace ZooLib

#endif // __ZooLib_CountedWithoutFinalize_h__