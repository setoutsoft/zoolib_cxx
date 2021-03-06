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

#ifndef __ZUtil_SystemConfiguration_h__
#define __ZUtil_SystemConfiguration_h__ 1
#include "zconfig.h"
#include "zoolib/ZCONFIG_SPI.h"

#if ZCONFIG_SPI_Enabled(SystemConfiguration)

#include "zoolib/Callable.h"

#include "zoolib/ZRef_CF.h"
#include "zoolib/ZYad.h"

#include <SystemConfiguration/SCDynamicStore.h>

namespace ZooLib {
namespace ZUtil_SystemConfiguration {

// =================================================================================================
#pragma mark -
#pragma mark Store

class Store : public ZCounted
	{
public:
	typedef Callable<void(ZRef<Store>,CFArrayRef)> Callable;

	Store(ZRef<Callable> iCallable);
	virtual ~Store();

// From ZCounted
	virtual void Initialize();

// Our protocol
	SCDynamicStoreRef GetStoreRef();

private:
	static void spCallback(SCDynamicStoreRef store, CFArrayRef changedKeys, void *info);

	const ZRef<Callable> fCallable;
	ZRef<SCDynamicStoreRef> fStoreRef;
	};

// =================================================================================================
#pragma mark -
#pragma mark sYadMapRPos

ZRef<ZYadMapRPos> sYadMapRPos(SCDynamicStoreRef iStoreRef, CFArrayRef iKeys);
ZRef<ZYadMapRPos> sYadMapRPos(SCDynamicStoreRef iStoreRef, const std::string& iPattern);
ZRef<ZYadMapRPos> sYadMapRPos(SCDynamicStoreRef iStoreRef);

} // namespace ZUtil_SystemConfiguration
} // namespace ZooLib

#endif // ZCONFIG_SPI_Enabled(SystemConfiguration)

#endif // __ZUtil_SystemConfiguration_h__
