/* -------------------------------------------------------------------------------------------------
Copyright (c) 2008 Andrew Green
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

#include "zoolib/ZFactoryChain.h"
#include "zoolib/ZYad.h"
#include "zoolib/ZMemoryBlock.h"

#include <stdio.h> // For printf

NAMESPACE_ZOOLIB_BEGIN

using std::min;
using std::string;
using std::vector;

/*
Yad is another neologism by Eric Cooper. As an acronym it stands for Yet Another Data, but
interestingly it is also a hebrew word. From <http://en.wikipedia.org/wiki/Yad>:

A yad (Hebrew: יד‎), literally, "hand," is a Jewish ritual pointer, used to point to the text during
the Torah reading from the parchment Torah scrolls. It is intended to prevent anyone from touching
the parchment, which is considered sacred; additionally, the fragile parchment can be damaged by
the oils of the skin. While not required when chanting from the Torah, it is used frequently.

An instance of ZYadR points within some data source, can be moved through that source, and can
return further instances of ZYadR referencing substructures of that source. Basically it's a
generic facility for accessing data that looks like ZooLib ZTuple suite -- CFDictionary,
NSDictionary, PList, XMLRPC, JSON, Javascript types etc.

The idea is that there are a Map-like and List-like entities in many APIs, and that abstracting
access to them allows code to be applied to any of them.
*/

// =================================================================================================
#pragma mark -
#pragma mark * ZYadOptions

ZYadOptions::ZYadOptions(bool iDoIndentation)
:	fRawChunkSize(16),
	fRawByteSeparator(" "),
	fRawAsASCII(iDoIndentation),
	fBreakStrings(true),
	fStringLineLength(80),
	fIDsHaveDecimalVersionComment(iDoIndentation),
	fTimesHaveUserLegibleComment(true)
	{
	if (iDoIndentation)
		{
		fEOLString = "\n";
		fIndentString = "  ";
		}
	}

// =================================================================================================
#pragma mark -
#pragma mark * ZYadParseException

ZYadParseException::ZYadParseException(const string& iWhat)
:	runtime_error(iWhat)
	{}

ZYadParseException::ZYadParseException(const char* iWhat)
:	runtime_error(iWhat)
	{}

// =================================================================================================
#pragma mark -
#pragma mark * ZYadR

ZYadR::ZYadR()
	{}

bool ZYadR::IsSimple(const ZYadOptions& iOptions)
	{ return true; }

ZRef<ZYadR> ZYadR::Meta()
	{ return ZRef<ZYadR>(); }

// =================================================================================================
#pragma mark -
#pragma mark * ZYadListR

bool ZYadListR::IsSimple(const ZYadOptions& iOptions)
	{
	return false;
	}

void ZYadListR::Skip()
	{ this->ReadInc(); }

void ZYadListR::SkipAll()
	{
	while (ZRef<ZYadR> cur = this->ReadInc())
		{}
	}

// =================================================================================================
#pragma mark -
#pragma mark * ZYadListRPos

bool ZYadListRPos::IsSimple(const ZYadOptions& iOptions)
	{
	uint64 theSize = this->GetSize();
	if (theSize == 0)
		return true;

	uint64 thePosition = this->GetPosition();
	if (thePosition == theSize - 1)
		{
		ZRef<ZYadR> theYadR = this->ReadInc();
		this->SetPosition(thePosition);
		return theYadR->IsSimple(iOptions);
		}

	return false;
	}

void ZYadListRPos::Skip()
	{
	uint64 theSize = this->GetSize();
	uint64 thePosition = this->GetPosition();
	this->SetPosition(min(theSize, thePosition + 1));
	}

void ZYadListRPos::SkipAll()
	{ this->SetPosition(this->GetSize()); }

// =================================================================================================
#pragma mark -
#pragma mark * ZYadMapR

bool ZYadMapR::IsSimple(const ZYadOptions& iOptions)
	{ return false; }

void ZYadMapR::Skip()
	{
	string dummy;
	this->ReadInc(dummy);
	}

void ZYadMapR::SkipAll()
	{
	string dummy;
	while (ZRef<ZYadR> cur = this->ReadInc(dummy))
		{}
	}

// =================================================================================================
#pragma mark -
#pragma mark * ZYadMapRPos

bool ZYadMapRPos::IsSimple(const ZYadOptions& iOptions)
	{
	if (ZRef<ZYadMapRPos> clone = this->Clone())
		{
		string dummy;
		if (ZRef<ZYadR> theYadR = clone->ReadInc(dummy))
			{
			if (!clone->ReadInc(dummy))
				{
				// We've exhausted ouselves, so we had just one entry.
				return theYadR->IsSimple(iOptions);
				}
			}
		else
			{
			return true;
			}
		}
	return false;
	}

NAMESPACE_ZOOLIB_END
