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

#ifndef __ZCallable_Ptr__
#define __ZCallable_Ptr__ 1
#include "zconfig.h"

#include "zoolib/ZCallable.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZCallable_Ptr_R0

template <class Callee, class R>
class ZCallable_Ptr_R0
:	public ZCallable_R0<R>
	{
public:
	typedef Roid (Callee::*Method_t)();

	ZCallable_Ptr_R0(Method_t iMethod, ZRef<Callee> iCallee)
	:	fMethod(iMethod)
	,	fCallee(iCallee)
		{}

	// From ZCallable_R0
	virtual R Invoke()
		{ return (fCallee->*fMethod)(); }

private:
	Method_t fMethod;
	Callee* fCallee;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZCallable_Ptr_R1

template <class Callee, class R, class P0>
class ZCallable_Ptr_R1
:	public ZCallable_R1<R, P0>
	{
public:
	typedef R (Callee::*Method_t)(P0);

	ZCallable_Ptr_R1(Method_t iMethod, ZRef<Callee> iCallee)
	:	fMethod(iMethod)
	,	fCallee(iCallee)
		{}

	// From ZCallable_R1
	virtual R Invoke(P0 iP0)
		{ return (fCallee->*fMethod)(iP0); }

private:
	Method_t fMethod;
	Callee* fCallee;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZCallable_Ptr_V0

template <class Callee>
class ZCallable_Ptr_V0
:	public ZCallable_V0
	{
public:
	typedef void (Callee::*Method_t)();

	ZCallable_Ptr_V0(Method_t iMethod, ZRef<Callee> iCallee)
	:	fMethod(iMethod)
	,	fCallee(iCallee)
		{}

	// From ZCallable_V0
	virtual void Invoke()
		{ (fCallee->*fMethod)(); }

private:
	Method_t fMethod;
	Callee* fCallee;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZCallable_Ptr_V1

template <class Callee, class P0>
class ZCallable_Ptr_V1
:	public ZCallable_V1<P0>
	{
public:
	typedef void (Callee::*Method_t)(P0);

	ZCallable_Ptr_V1(Method_t iMethod, ZRef<Callee> iCallee)
	:	fMethod(iMethod)
	,	fCallee(iCallee)
		{}

	// From ZCallable_V1
	virtual void Invoke(P0 iP0)
		{ (fCallee->*fMethod)(iP0); }

private:
	Method_t fMethod;
	Callee* fCallee;
	};

} // namespace ZooLib

#endif // __ZCallable_Ptr__
