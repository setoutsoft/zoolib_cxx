/* -------------------------------------------------------------------------------------------------
Copyright (c) 2011 Andrew Green
http://www.zoolib.org

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,Publish, distribute,
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

#ifndef __ZooLib_Caller_CarbonEvents_h__
#define __ZooLib_Caller_CarbonEvents_h__ 1
#include "zconfig.h"
#include "zoolib/ZCONFIG_SPI.h"

#include "zoolib/Caller_EventLoop.h"

#if ZCONFIG_SPI_Enabled(Carbon64)

namespace ZooLib {

// =================================================================================================
// MARK: - Caller_CarbonEvents

class Caller_CarbonEvents
:	public Caller_EventLoop
	{
protected:
	static ZRef<Caller_CarbonEvents> sGet();

// From Caller_EventLoop
	virtual bool pTrigger();

private:
	static void spCallback(void* iRefcon);
	};

} // namespace ZooLib

#endif // ZCONFIG_SPI_Enabled(Carbon64)

#endif // __ZooLib_Caller_CarbonEvents_h__