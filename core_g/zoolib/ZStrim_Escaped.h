/* -------------------------------------------------------------------------------------------------
Copyright (c) 2009 Andrew Green
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

#ifndef __ZStrim_Escaped_h__
#define __ZStrim_Escaped_h__ 1
#include "zconfig.h"

#include "zoolib/ChanW_UTF_More.h"

#include "zoolib/ZStrim.h"

namespace ZooLib {

// =================================================================================================
// MARK: - ZStrimR_Escaped

class ZStrimR_Escaped : public ZStrimR
	{
public:
	ZStrimR_Escaped(const ZStrimU& iStrimSource, UTF32 iDelimiter);
	~ZStrimR_Escaped();

// From ZStrimR
	virtual void Imp_ReadUTF32(UTF32* oDest, size_t iCount, size_t* oCount);

protected:
	const ZStrimU& fStrimSource;
	UTF32 fDelimiter;
	};

// =================================================================================================
// MARK: - ZStrimW_Escaped

/// A write filter strim that inserts C-style escape sequences.

class ZStrimW_Escaped : public ChanW_UTF_Native32
	{
public:
	struct Options
		{
		Options();

		string8 fEOL;
		bool fQuoteQuotes;
		bool fEscapeHighUnicode;
		};

	ZStrimW_Escaped(const Options& iOptions, const ChanW_UTF& iStrimSink);
	ZStrimW_Escaped(const ChanW_UTF& iStrimSink);
	~ZStrimW_Escaped();

// From ChanW_UTF_Native32
	virtual size_t QWrite(const UTF32* iSource, size_t iCountCU);

private:
	const ChanW_UTF& fStrimSink;
	string8 fEOL;
	bool fQuoteQuotes;
	bool fEscapeHighUnicode;
	bool fLastWasCR;
	};

// =================================================================================================

} // namespace ZooLib

#endif // __ZStrim_Escaped_h__
