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

#ifndef __ZSaveRestore_T__
#define __ZSaveRestore_T__ 1
#include "zconfig.h"

#include "zoolib/ZCompat_algorithm.h" // For std::swap

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZSaveRestore_T

template <class T>
class ZSaveRestore_T
	{
public:
	ZSaveRestore_T(T& ioRef)
	:	fRef(ioRef)
	,	fValPrior(ioRef)
		{}

	~ZSaveRestore_T()
		{ std::swap(fRef, fValPrior); }

	const T& GetPrior() const
		{ return fValPrior; }

private:
	T& fRef;
	T fValPrior;
	};

} // namespace ZooLib

#endif // __ZSaveRestore_T__