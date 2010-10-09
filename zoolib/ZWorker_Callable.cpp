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

#include "zoolib/ZWorker_Callable.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZWorker_Callable

ZWorker_Callable::ZWorker_Callable(const ZRef<Callable_t>& iCallable)
:	fCallable(iCallable)
	{}

bool ZWorker_Callable::Work()
	{
	if (ZRef<Callable_t> theCallable = fCallable)
		return fCallable->Call(this);
	return false;
	}

ZRef<ZWorker> MakeWorker(ZRef<ZWorker_Callable::Callable_t> iCallable)
	{ return new ZWorker_Callable(iCallable); }

// =================================================================================================
#pragma mark -
#pragma mark * ZWorker_Callable_Once

ZWorker_Callable_Once::ZWorker_Callable_Once(const ZRef<Callable_t>& iCallable)
:	fCallable(iCallable)
	{}

bool ZWorker_Callable_Once::Work()
	{ return false; }

void ZWorker_Callable_Once::RunnerDetached()
	{
	if (ZRef<Callable_t> theCallable = fCallable)
		fCallable->Call();
	}

ZRef<ZWorker> MakeWorker(ZRef<ZWorker_Callable_Once::Callable_t> iCallable)
	{ return new ZWorker_Callable_Once(iCallable); }

ZRef<ZWorker> MakeWorkerOnce(ZRef<ZWorker_Callable_Once::Callable_t> iCallable)
	{ return new ZWorker_Callable_Once(iCallable); }

} // namespace ZooLib