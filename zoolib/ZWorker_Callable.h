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

#ifndef __ZWorker_Callable__
#define __ZWorker_Callable__ 1
#include "zconfig.h"

#include "zoolib/ZCallable.h"
#include "zoolib/ZWorker.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZWorker_Callable

class ZWorker_Callable
:	public virtual ZWorker
	{
public:
	typedef ZCallable<bool(ZRef_ZWorker)> Callable_t;

	ZWorker_Callable(const ZRef<Callable_t>& iCallable)
	:	fCallable(iCallable)
		{}

	virtual bool Work()
		{
		if (ZRef<Callable_t> theCallable = fCallable)
			return fCallable->Call(this);
		return false;
		}

private:
	ZRef<Callable_t> fCallable;
	};

inline ZRef<ZWorker> MakeWorker(
	const ZRef<ZCallable<bool(ZRef_ZWorker)> >& iCallable)
	{ return new ZWorker_Callable(iCallable); }

// =================================================================================================
#pragma mark -
#pragma mark * ZWorker_Callable_Once

class ZWorker_Callable_Once
:	public ZWorker
	{
public:
	typedef ZCallable<void(void)> Callable_t;

	ZWorker_Callable_Once(const ZRef<Callable_t>& iCallable)
	:	fCallable(iCallable)
		{}

	virtual bool Work()
		{ return false; }

	virtual void RunnerDetached()
		{
		if (ZRef<Callable_t> theCallable = fCallable)
			fCallable->Call();
		}

private:
	ZRef<Callable_t> fCallable;
	};

// Maker that makes explicit this is will be a one-shot worker.
inline ZRef<ZWorker> MakeWorkerOnce(
	const ZRef<ZCallable<void(void)> >& iCallable)
	{ return new ZWorker_Callable_Once(iCallable); }

inline ZRef<ZWorker> MakeWorker(
	const ZRef<ZCallable<void(void)> >& iCallable)
	{ return new ZWorker_Callable_Once(iCallable); }

} // namespace ZooLib

#endif // __ZWorker_Callable__
