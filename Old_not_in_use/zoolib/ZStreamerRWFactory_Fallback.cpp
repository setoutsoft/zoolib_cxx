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

#include "zoolib/ZStreamerRWFactory_Fallback.h"

namespace ZooLib {

using std::vector;

// =================================================================================================
#pragma mark -
#pragma mark ZStreamerRWFactory_Fallback

ZStreamerRWFactory_Fallback::ZStreamerRWFactory_Fallback()
	{}

ZStreamerRWFactory_Fallback::ZStreamerRWFactory_Fallback(
	const ZRef<ZStreamerRWFactory>* iFactories, size_t iCount)
:	fFactories(iFactories, iFactories + iCount)
	{}

ZStreamerRWFactory_Fallback::~ZStreamerRWFactory_Fallback()
	{}

ZRef<ZStreamerRW> ZStreamerRWFactory_Fallback::MakeStreamerRW()
	{
	for (vector<ZRef<ZStreamerRWFactory> >::iterator ii = fFactories.begin();
		ii != fFactories.end(); ++ii)
		{
		if (ZRef<ZStreamerRW> theSRW = (*ii)->MakeStreamerRW())
			return theSRW;
		}

	return null;
	}

void ZStreamerRWFactory_Fallback::Add(ZRef<ZStreamerRWFactory> iFactory)
	{ fFactories.push_back(iFactory); }

void ZStreamerRWFactory_Fallback::Add(const ZRef<ZStreamerRWFactory>* iFactories, size_t iCount)
	{
	if (iFactories && iCount)
		fFactories.insert(fFactories.end(), iFactories, iFactories + iCount);
	}

} // namespace ZooLib