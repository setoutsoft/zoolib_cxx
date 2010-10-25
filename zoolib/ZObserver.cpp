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

#include "zoolib/ZObserver.h"

#if ZCONFIG_SPI_Enabled(CoreFoundation)

#include "zoolib/ZUtil_CFType.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark * ZObserver

ZObserver::ZObserver(void* iObject, const std::string& iName, ZRef<Callable> iCallable)
:	fObject(iObject)
,	fName(iName)
,	fCallable(iCallable)
	{}

ZObserver::~ZObserver()
	{}

void ZObserver::Initialize()
	{
	ZCounted::Initialize();

	::CFNotificationCenterAddObserver(
		::CFNotificationCenterGetLocalCenter(),
		this,
		spCallback,
		ZUtil_CFType::sString(fName),
		fObject,
		CFNotificationSuspensionBehaviorDeliverImmediately);
	}

void ZObserver::Finalize()
	{
	::CFNotificationCenterRemoveEveryObserver(::CFNotificationCenterGetLocalCenter(), this);

	ZCounted::Finalize();
	}

void* ZObserver::GetObject()
	{ return fObject; }

std::string ZObserver::GetName()
	{ return fName; }

void ZObserver::spCallback(CFNotificationCenterRef center,
	void *observer, CFStringRef name, const void *object, CFDictionaryRef userInfo)
	{
	ZRef<ZObserver> theObserver = static_cast<ZObserver*>(observer);
	theObserver->fCallable->Call(theObserver, userInfo);
	}

} // namespace ZooLib

#endif // ZCONFIG_SPI_Enabled(CoreFoundation)