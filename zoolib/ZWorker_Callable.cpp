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

ZWorker_Callable::~ZWorker_Callable()
	{}

bool ZWorker_Callable::Work()
	{ return sCall(fCallable, this); }

ZRef<ZWorker> sWorker(ZRef<ZWorker_Callable::Callable_t> iCallable)
	{ return new ZWorker_Callable(iCallable); }

ZRef<ZWorker> sWorker(ZRef<ZWorker_Callable::Callable_t> iCallable,
	ZRef<ZWorker::Callable_Detached_t> iCallable_Detached)
	{
	ZRef<ZWorker> theWorker = new ZWorker_Callable(iCallable);
	theWorker->GetSetCallable_Detached(iCallable_Detached);
	return theWorker;
	}

} // namespace ZooLib
