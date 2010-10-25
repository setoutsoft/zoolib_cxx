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

#ifndef __ZDelegate__
#define __ZDelegate__ 1
#include "zconfig.h"

#ifdef __OBJC__

#include "zoolib/ZCallable.h"
#include "zoolib/ZCompat_NonCopyable.h"
#include "zoolib/ZRef_NSObject.h"

#import <Foundation/NSInvocation.h>
#import <Foundation/NSMethodSignature.h>

#include <map>
#include <string>

@class ZooLib_ZDelegate_Proxy;

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZDelegate

class ZDelegate : ZooLib::NonCopyable
	{
public:
	ZDelegate();
	~ZDelegate();

	template <class Callable>
	void Add(SEL iSEL, ZRef<Callable> iCallable);

	operator id();

// Can't be physically private, access needed by ZooLib_ZDelegate_Proxy.
	BOOL pRespondsToSelector(SEL aSelector);
	void pForwardInvocation(NSInvocation* anInvocation);
	NSMethodSignature* pMethodSignatureForSelector(SEL aSelector);

private:
	class Wrapper;
	template <class Signature> class Wrapper_T;

	ZooLib_ZDelegate_Proxy* fProxy;
	std::map<SEL, Wrapper*> fWrappers;

	template <class R>
	static Wrapper* spMakeWrapper(ZRef<ZCallable<R(void)> > iCallable);

	template <class R,
		class P0>
	static Wrapper* spMakeWrapper(ZRef<ZCallable<R(P0)> > iCallable);
	
	template <class R,
		class P0, class P1>
	static Wrapper* spMakeWrapper(ZRef<ZCallable<R(P0,P1)> > iCallable);

	template <class R,
		class P0, class P1, class P2>
	static Wrapper* spMakeWrapper(ZRef<ZCallable<R(P0,P1,P2)> > iCallable);

	template <class R,
		class P0, class P1, class P2, class P3>
	static Wrapper* spMakeWrapper(ZRef<ZCallable<R(P0,P1,P2,P3)> > iCallable);

	template <class R,
		class P0, class P1, class P2, class P3, class P4>
	static Wrapper* spMakeWrapper(ZRef<ZCallable<R(P0,P1,P2,P3,P4)> > iCallable);
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZDelegate::Wrapper

class ZDelegate::Wrapper
	{
	friend class ZDelegate;

	virtual ~Wrapper() {}
	virtual NSMethodSignature* MethodSignature() = 0;
	virtual void ForwardInvocation(NSInvocation* anInvocation) = 0;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZDelegate::Wrapper_T (specialized for 0 params)

template <class R>
class ZDelegate::Wrapper_T<R(void)> : public ZDelegate::Wrapper
	{
	friend class ZDelegate;

	typedef ZCallable<R(void)> Callable;

	Wrapper_T(ZRef<Callable> iCallable)
	:	fCallable(iCallable)
		{
		std::string signature;
		signature += @encode(R);
		signature += "@:";
		fNSMethodSignature = [NSMethodSignature signatureWithObjCTypes:signature.c_str()];
		}
	
	virtual NSMethodSignature* MethodSignature()
		{ return fNSMethodSignature; }

	virtual void ForwardInvocation(NSInvocation* anInvocation)
		{
		R result = fCallable->Call();
		[anInvocation setReturnValue:&result];
		}

	ZRef<Callable> fCallable;
	ZRef<NSMethodSignature> fNSMethodSignature;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZDelegate::Wrapper_T (specialized for 1 param)

template <class R,
	class P0>
class ZDelegate::Wrapper_T<R(P0)> : public ZDelegate::Wrapper
	{
	friend class ZDelegate;

	typedef ZCallable<R(P0)> Callable;

	Wrapper_T(ZRef<Callable> iCallable)
	:	fCallable(iCallable)
		{
		std::string signature;
		signature += @encode(R);
		signature += "@:";
		signature += @encode(P0);
		fNSMethodSignature = [NSMethodSignature signatureWithObjCTypes:signature.c_str()];
		}
	
	virtual NSMethodSignature* MethodSignature()
		{ return fNSMethodSignature; }

	virtual void ForwardInvocation(NSInvocation* anInvocation)
		{
		P0 p0;
		[anInvocation getArgument:&p0 atIndex:2];

		R result = fCallable->Call(p0);
		[anInvocation setReturnValue:&result];
		}

	ZRef<Callable> fCallable;
	ZRef<NSMethodSignature> fNSMethodSignature;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZDelegate::Wrapper_T (specialized for 2 params)

template <class R,
	class P0, class P1>
class ZDelegate::Wrapper_T<R(P0,P1)> : public ZDelegate::Wrapper
	{
	friend class ZDelegate;

	typedef ZCallable<R(P0,P1)> Callable;

	Wrapper_T(ZRef<Callable> iCallable)
	:	fCallable(iCallable)
		{
		std::string signature;
		signature += @encode(R);
		signature += "@:";
		signature += @encode(P0);
		signature += @encode(P1);
		fNSMethodSignature = [NSMethodSignature signatureWithObjCTypes:signature.c_str()];
		}
	
	virtual NSMethodSignature* MethodSignature()
		{ return fNSMethodSignature; }

	virtual void ForwardInvocation(NSInvocation* anInvocation)
		{
		P0 p0;
		[anInvocation getArgument:&p0 atIndex:2];
		P1 p1;
		[anInvocation getArgument:&p1 atIndex:3];

		R result = fCallable->Call(p0, p1);
		[anInvocation setReturnValue:&result];
		}

	ZRef<Callable> fCallable;
	ZRef<NSMethodSignature> fNSMethodSignature;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZDelegate::Wrapper_T (specialized for 3 params)

template <class R,
	class P0, class P1, class P2>
class ZDelegate::Wrapper_T<R(P0,P1,P2)> : public ZDelegate::Wrapper
	{
	friend class ZDelegate;

	typedef ZCallable<R(P0,P1,P2)> Callable;

	Wrapper_T(ZRef<Callable> iCallable)
	:	fCallable(iCallable)
		{
		std::string signature;
		signature += @encode(R);
		signature += "@:";
		signature += @encode(P0);
		signature += @encode(P1);
		signature += @encode(P2);
		fNSMethodSignature = [NSMethodSignature signatureWithObjCTypes:signature.c_str()];
		}
	
	virtual NSMethodSignature* MethodSignature()
		{ return fNSMethodSignature; }

	virtual void ForwardInvocation(NSInvocation* anInvocation)
		{
		P0 p0;
		[anInvocation getArgument:&p0 atIndex:2];
		P1 p1;
		[anInvocation getArgument:&p1 atIndex:3];
		P2 p2;
		[anInvocation getArgument:&p2 atIndex:4];

		R result = fCallable->Call(p0, p1, p2);
		[anInvocation setReturnValue:&result];
		}

	ZRef<Callable> fCallable;
	ZRef<NSMethodSignature> fNSMethodSignature;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZDelegate::Wrapper_T (specialized for 4 params)

template <class R,
	class P0, class P1, class P2, class P3>
class ZDelegate::Wrapper_T<R(P0,P1,P2,P3)> : public ZDelegate::Wrapper
	{
	friend class ZDelegate;

	typedef ZCallable<R(P0,P1,P2,P3)> Callable;

	Wrapper_T(ZRef<Callable> iCallable)
	:	fCallable(iCallable)
		{
		std::string signature;
		signature += @encode(R);
		signature += "@:";
		signature += @encode(P0);
		signature += @encode(P1);
		signature += @encode(P2);
		signature += @encode(P3);
		fNSMethodSignature = [NSMethodSignature signatureWithObjCTypes:signature.c_str()];
		}
	
	virtual NSMethodSignature* MethodSignature()
		{ return fNSMethodSignature; }

	virtual void ForwardInvocation(NSInvocation* anInvocation)
		{
		P0 p0;
		[anInvocation getArgument:&p0 atIndex:2];
		P1 p1;
		[anInvocation getArgument:&p1 atIndex:3];
		P2 p2;
		[anInvocation getArgument:&p2 atIndex:4];
		P3 p3;
		[anInvocation getArgument:&p3 atIndex:5];

		R result = fCallable->Call(p0, p1, p2, p3);
		[anInvocation setReturnValue:&result];
		}

	ZRef<Callable> fCallable;
	ZRef<NSMethodSignature> fNSMethodSignature;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZDelegate::Wrapper_T (specialized for 5 params)

template <class R,
	class P0, class P1, class P2, class P3, class P4>
class ZDelegate::Wrapper_T<R(P0,P1,P2,P3,P4)> : public ZDelegate::Wrapper
	{
	friend class ZDelegate;

	typedef ZCallable<R(P0,P1,P2,P3,P4)> Callable;

	Wrapper_T(ZRef<Callable> iCallable)
	:	fCallable(iCallable)
		{
		std::string signature;
		signature += @encode(R);
		signature += "@:";
		signature += @encode(P0);
		signature += @encode(P1);
		signature += @encode(P2);
		signature += @encode(P3);
		signature += @encode(P4);
		fNSMethodSignature = [NSMethodSignature signatureWithObjCTypes:signature.c_str()];
		}
	
	virtual NSMethodSignature* MethodSignature()
		{ return fNSMethodSignature; }

	virtual void ForwardInvocation(NSInvocation* anInvocation)
		{
		P0 p0;
		[anInvocation getArgument:&p0 atIndex:2];
		P1 p1;
		[anInvocation getArgument:&p1 atIndex:3];
		P2 p2;
		[anInvocation getArgument:&p2 atIndex:4];
		P3 p3;
		[anInvocation getArgument:&p3 atIndex:5];
		P4 p4;
		[anInvocation getArgument:&p4 atIndex:6];

		R result = fCallable->Call(p0, p1, p2, p3, p4);
		[anInvocation setReturnValue:&result];
		}

	ZRef<Callable> fCallable;
	ZRef<NSMethodSignature> fNSMethodSignature;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZDelegate::spMakeWrapper

template <class R,
	class P0>
ZDelegate::Wrapper* ZDelegate::spMakeWrapper(ZRef<ZCallable<R(P0)> > iCallable)
	{ return new ZDelegate::Wrapper_T<R(P0)>(iCallable); }

template <class R,
	class P0, class P1>
ZDelegate::Wrapper* ZDelegate::spMakeWrapper(ZRef<ZCallable<R(P0,P1)> > iCallable)
	{ return new ZDelegate::Wrapper_T<R(P0,P1)>(iCallable); }

template <class R,
	class P0, class P1, class P2>
ZDelegate::Wrapper* ZDelegate::spMakeWrapper(ZRef<ZCallable<R(P0,P1,P2)> > iCallable)
	{ return new ZDelegate::Wrapper_T<R(P0,P1,P2)>(iCallable); }

template <class R,
	class P0, class P1, class P2, class P3>
ZDelegate::Wrapper* ZDelegate::spMakeWrapper(ZRef<ZCallable<R(P0,P1,P2,P3)> > iCallable)
	{ return new ZDelegate::Wrapper_T<R(P0,P1,P2,P3)>(iCallable); }

template <class R,
	class P0, class P1, class P2, class P3, class P4>
ZDelegate::Wrapper* ZDelegate::spMakeWrapper(ZRef<ZCallable<R(P0,P1,P2,P3,P4)> > iCallable)
	{ return new ZDelegate::Wrapper_T<R(P0,P1,P2,P3,P4)>(iCallable); }

// =================================================================================================
#pragma mark -
#pragma mark * ZDelegate::Add

template <class Callable>
void ZDelegate::Add(SEL iSEL, ZRef<Callable> iCallable)
	{
	ZAssert(fWrappers.end() == fWrappers.find(iSEL));
	fWrappers[iSEL] = spMakeWrapper(iCallable);
	}

} // namespace ZooLib

#endif // __OBJC__
#endif // __ZDelegate__