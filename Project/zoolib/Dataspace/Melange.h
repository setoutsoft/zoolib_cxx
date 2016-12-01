/* -------------------------------------------------------------------------------------------------
Copyright (c) 2014 Andrew Green
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

#ifndef __ZooLib_Dataspace_Melange_h__
#define __ZooLib_Dataspace_Melange_h__ 1
#include "zconfig.h"

#include "zoolib/Multi.h"
#include "zoolib/Starter.h"

#include "zooLib/Dataspace/RelsWatcher.h"
#include "zoolib/DatonSet/DatonSet.h"

// =================================================================================================
#pragma mark -

namespace ZooLib {
namespace Dataspace {

using RelsWatcher::Callable_Register;
using DatonSet::Callable_DatonSetUpdate;

typedef Multi3<
	ZRef<Callable_Register>,
	ZRef<Callable_DatonSetUpdate>,
	ZRef<Starter>
	> Melange_t;

typedef Callable<Melange_t()> Factory_Melange;

} // namespace Dataspace
} // namespace ZooLib

#endif // __ZooLib_Dataspace_Melange_h__