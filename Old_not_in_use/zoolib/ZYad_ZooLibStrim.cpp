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

#include "zoolib/Unicode.h"
#include "zoolib/Util_Chan_UTF_Operators.h"
#include "zoolib/Util_Time.h"

#include "zoolib/ZGeomPOD.h"
#include "zoolib/ZStreamW_HexStrim.h"
#include "zoolib/ZStrimR_Boundary.h"
#include "zoolib/ZStrim_Escaped.h"
#include "zoolib/ZUtil_Strim.h"
#include "zoolib/ZYad_ZooLibStrim.h"

#include <vector>

namespace ZooLib {

using std::min;
using std::string;
using std::vector;

// =================================================================================================
#pragma mark -
#pragma mark Static parsing functions

static void spThrowParseException(const string& iMessage)
	{
	throw ZYadParseException_ZooLibStrim(iMessage);
	}

static bool spTryRead_PropertyName(const ZStrimU& iStrimU, Name& oName)
	{
	string8 theString;
	using namespace ZUtil_Strim;

	if (not sTryRead_EscapedString(iStrimU, '"', theString))
		{
		if (not sTryRead_EscapedString(iStrimU, '\'', theString))
			{
			if (not ZYad_ZooLibStrim::sRead_Identifier(iStrimU, nullptr, &theString))
				return false;
			}
		}

	oName = theString;
	return true;
	}

static int64 spMustRead_GenericInteger(const ZStrimU& iStrimU)
	{
	using namespace ZUtil_Strim;
	int64 theInteger;
	if (not sTryRead_SignedGenericInteger(iStrimU, theInteger))
		spThrowParseException("Expected an integer");
	return theInteger;
	}

static void spMustRead_WSCommaWS(const ZStrimU& iStrimU)
	{
	using namespace ZUtil_Strim;
	sSkip_WSAndCPlusPlusComments(iStrimU);

	if (not sTryRead_CP(iStrimU, ','))
		spThrowParseException("Expected a ','");

	sSkip_WSAndCPlusPlusComments(iStrimU);
	}

static bool spFromStrim_Value(const ZStrimU& iStrimU, ZAny& oVal)
	{
	using namespace ZUtil_Strim;

	sSkip_WSAndCPlusPlusComments(iStrimU);

	if (false)
		{}
#if 0//##
	else if (sTryRead_CP(iStrimU, '@'))
		{
		// It's a property name.
		string propertyName;
		if (not sTryRead_EscapedString(iStrimU, '"', propertyName))
			{
			if (not sTryRead_EscapedString(iStrimU, '\'', propertyName))
				{
				if (not ZYad_ZooLibStrim::sRead_Identifier(iStrimU, nullptr, &propertyName))
					spThrowParseException("Expected a property name after '#'");
				}
			}
		oVal.SetName(propertyName);
		}
#endif
	else
		{
		string theTypeLC, theType;
		if (not ZYad_ZooLibStrim::sRead_Identifier(iStrimU, &theTypeLC, &theType))
			{
			// We couldn't find any of the special characters, nor could
			// we read a type designator, so we fail to read a value,
			// which is not a parse error at this stage -- it might be for our caller.
			return false;
			}

		if (theTypeLC == "null")
			{
			oVal = ZAny();
			}
		else if (theTypeLC == "false")
			{
			oVal = false;
			}
		else if (theTypeLC == "true")
			{
			oVal = true;
			}
		else
			{
			sSkip_WSAndCPlusPlusComments(iStrimU);
			if (not sTryRead_CP(iStrimU, '('))
				spThrowParseException("Expected '(' following a type designator");

			sSkip_WSAndCPlusPlusComments(iStrimU);

			if (false)
				{}
#if 0
			else if (theTypeLC == "type")
				{
				string typeValueLC, typeValue;
				if (not ZYad_ZooLibStrim::sRead_Identifier(iStrimU, &typeValueLC, &typeValue))
					spThrowParseException("Expected a type name");

				if (typeValueLC == "null") oVal = eZType_Null;
				else if (typeValueLC == "string") oVal = eZType_String;
// AG 2007-08-06. cstring has never been used. Not even sure what it's point was.
//				else if (typeValueLC == "cstring") oVal = eZType_CString;
				else if (typeValueLC == "int8") oVal = eZType_Int8;
				else if (typeValueLC == "int16") oVal = eZType_Int16;
				else if (typeValueLC == "int32") oVal = eZType_Int32;
				else if (typeValueLC == "int64") oVal = eZType_Int64;
				else if (typeValueLC == "float") oVal = eZType_Float;
				else if (typeValueLC == "double") oVal = eZType_Double;
				else if (typeValueLC == "time") oVal = eZType_Time;
				else if (typeValueLC == "bool") oVal = eZType_Bool;
				else if (typeValueLC == "pointer") oVal = eZType_Pointer;
				else if (typeValueLC == "raw") oVal = eZType_Raw;
				else if (typeValueLC == "tuple") oVal = eZType_Tuple;
				else if (typeValueLC == "refcounted") oVal = eZType_RefCounted;
				else if (typeValueLC == "rect") oVal = eZType_Rect;
				else if (typeValueLC == "point") oVal = eZType_Point;
				else if (typeValueLC == "region") oVal = eZType_Region;
				else if (typeValueLC == "id") oVal = eZType_ID;
				else if (typeValueLC == "vector") oVal = eZType_Vector;
				else if (typeValueLC == "type") oVal = eZType_Type;
//##				else if (typeValueLC == "name") oVal = eZType_Name;
				else
					spThrowParseException("Unknown type name '" + typeValue + "'");
				}
#endif
			else if (theTypeLC == "id")
				{
				oVal = uint64(spMustRead_GenericInteger(iStrimU));
				}
			else if (theTypeLC == "int8")
				{
				oVal = int8(spMustRead_GenericInteger(iStrimU));
				}
			else if (theTypeLC == "int16")
				{
				oVal = int16(spMustRead_GenericInteger(iStrimU));
				}
			else if (theTypeLC == "int32")
				{
				oVal = int32(spMustRead_GenericInteger(iStrimU));
				}
			else if (theTypeLC == "int64")
				{
				oVal = int64(spMustRead_GenericInteger(iStrimU));
				}
			else if (theTypeLC == "bool")
				{
				string theBool;
				if (not ZYad_ZooLibStrim::sRead_Identifier(iStrimU, &theBool, nullptr))
					spThrowParseException("Expected 'false' or 'true'");

				if (theBool == "true")
					oVal = true;
				else if (theBool == "false")
					oVal = false;
				else
					spThrowParseException("Expected 'false' or 'true'");
				}
			else if (theTypeLC == "float")
				{
				double theDouble;
				if (not sTryRead_SignedDouble(iStrimU, theDouble))
					spThrowParseException("Expected a floating point number");
				oVal = float(theDouble);
				}
			else if (theTypeLC == "double")
				{
				double theDouble;
				if (not sTryRead_SignedDouble(iStrimU, theDouble))
					spThrowParseException("Expected a floating point number");
				oVal = theDouble;
				}
			else if (theTypeLC == "time")
				{
				if (sTryRead_CP(iStrimU, ')'))
					{
					// It's a time with no content, hence an invalid time.
					oVal = ZTime();

					// We'll take an early exit so the normal code that
					// looks for a closing parenthesis doesn't choke.
					return true;
					}

				// Read up till the closing paren, if any.
				const string8 theContent = sRead_Until(iStrimU, ')');
				if (const ZTime theTime = Util_Time::sFromString_ISO8601(theContent))
					{
					oVal = theTime;
					return true;
					}

				double theDouble;
				if (sTryRead_SignedDouble(ZStrimU_String(theContent), theDouble))
					{
					oVal = ZTime(theDouble);
					return true;
					}

				spThrowParseException("Expected a floating point time");
				}
			else if (theTypeLC == "rect")
				{
				ZRectPOD theRect;

				theRect.left = spMustRead_GenericInteger(iStrimU);

				spMustRead_WSCommaWS(iStrimU);

				theRect.top = spMustRead_GenericInteger(iStrimU);

				spMustRead_WSCommaWS(iStrimU);

				theRect.right = spMustRead_GenericInteger(iStrimU);

				spMustRead_WSCommaWS(iStrimU);

				theRect.bottom = spMustRead_GenericInteger(iStrimU);

				oVal = theRect;
				}
			else if (theTypeLC == "point")
				{
				ZPointPOD thePoint;

				thePoint.h = spMustRead_GenericInteger(iStrimU);

				spMustRead_WSCommaWS(iStrimU);

				thePoint.v = spMustRead_GenericInteger(iStrimU);

				oVal = thePoint;
				}
			else
				{
				spThrowParseException("Unknown type designator '" + theType + "'");
				}

			sSkip_WSAndCPlusPlusComments(iStrimU);

			if (not sTryRead_CP(iStrimU, ')'))
				spThrowParseException("Expected ')' to close a value");
			}
		}
	return true;
	}

static ZRef<ZYadR> spMakeYadR_ZooLibStrim(ZRef<ZStrimmerU> iStrimmerU)
	{
	using namespace ZUtil_Strim;

	const ZStrimU& theStrimU = iStrimmerU->GetStrimU();

	sSkip_WSAndCPlusPlusComments(theStrimU);

	if (sTryRead_CP(theStrimU, '['))
		{
		return new ZYadSeqR_ZooLibStrim(iStrimmerU, true);
		}
	else if (sTryRead_CP(theStrimU, '{'))
		{
		return new ZYadMapR_ZooLibStrim(iStrimmerU, true);
		}
	else if (sTryRead_CP(theStrimU, '('))
		{
		return new ZYadStreamerR_ZooLibStrim(iStrimmerU, true);
		}
	else if (sTryRead_CP(theStrimU, '"'))
		{
		return new ZYadStrimmerR_ZooLibStrim_Quote(iStrimmerU);
		}
	else if (sTryRead_CP(theStrimU, '\''))
		{
		return new ZYadStrimmerR_ZooLibStrim_Apos(iStrimmerU);
		}
	else
		{
		ZAny theVal;
		if (spFromStrim_Value(theStrimU, theVal))
			return sMake_YadAtomR_Any(theVal);
		}

	return null;
	}

// =================================================================================================
#pragma mark -
#pragma mark ZYadParseException_ZooLibStrim

ZYadParseException_ZooLibStrim::ZYadParseException_ZooLibStrim(const string& iWhat)
:	ZYadParseException_Std(iWhat)
	{}

ZYadParseException_ZooLibStrim::ZYadParseException_ZooLibStrim(const char* iWhat)
:	ZYadParseException_Std(iWhat)
	{}

// =================================================================================================
#pragma mark -
#pragma mark ZYadStreamerR_ZooLibStrim

ZYadStreamerR_ZooLibStrim::ZYadStreamerR_ZooLibStrim(ZRef<ZStrimmerU> iStrimmerU, bool iReadDelimiter)
:	fStrimmerU(iStrimmerU),
	fReadDelimiter(iReadDelimiter),
	fStreamR(fStrimmerU->GetStrimU())
	{}

void ZYadStreamerR_ZooLibStrim::Finish()
	{
	using namespace ZUtil_Strim;

	fStreamR.SkipAll();

	if (fReadDelimiter)
		{
		const ZStrimU& theStrimU = fStrimmerU->GetStrimU();
		sSkip_WSAndCPlusPlusComments(theStrimU);
		if (not sTryRead_CP(theStrimU, ')'))
			spThrowParseException("Expected ')' to close a raw");
		}
	}

const ZStreamR& ZYadStreamerR_ZooLibStrim::GetStreamR()
	{ return fStreamR; }

// =================================================================================================
#pragma mark -
#pragma mark ZYadStrimmerR_ZooLibStrim_Apos

ZYadStrimmerR_ZooLibStrim_Apos::ZYadStrimmerR_ZooLibStrim_Apos(ZRef<ZStrimmerU> iStrimmerU)
:	fStrimmerU(iStrimmerU),
	fStrimR(fStrimmerU->GetStrimU(), '\'')
	{}

void ZYadStrimmerR_ZooLibStrim_Apos::Finish()
	{
	using namespace ZUtil_Strim;
	fStrimR.SkipAll();
	if (not sTryRead_CP(fStrimmerU->GetStrimU(), '\''))
		throw ParseException("Missing string delimiter");
	}

const ZStrimR& ZYadStrimmerR_ZooLibStrim_Apos::GetStrimR()
	{ return fStrimR; }

// =================================================================================================
#pragma mark -
#pragma mark ZYadStrimmerR_ZooLibStrim_Quote

ZYadStrimmerR_ZooLibStrim_Quote::ZYadStrimmerR_ZooLibStrim_Quote(ZRef<ZStrimmerU> iStrimmerU)
:	fStrimmerU(iStrimmerU),
	fStrimR_Boundary("\"\"\"", fStrimmerU->GetStrimR()),
	fQuotesSeen(1)
	{}

void ZYadStrimmerR_ZooLibStrim_Quote::Finish()
	{ this->SkipAll(); }

const ZStrimR& ZYadStrimmerR_ZooLibStrim_Quote::GetStrimR()
	{ return *this; }

void ZYadStrimmerR_ZooLibStrim_Quote::Imp_ReadUTF32(UTF32* oDest, size_t iCount, size_t* oCount)
	{
	using namespace ZUtil_Strim;

	const ZStrimU& theStrimU = fStrimmerU->GetStrimU();

	UTF32* localDest = oDest;
	UTF32* localDestEnd = oDest + iCount;
	bool exhausted = false;
	while (localDestEnd > localDest && !exhausted)
		{
		switch (fQuotesSeen)
			{
			case 0:
				{
				sSkip_WSAndCPlusPlusComments(theStrimU);

				if (sTryRead_CP(theStrimU, '"'))
					fQuotesSeen = 1;
				else
					exhausted = true;
				break;
				}
			case 1:
				{
				if (sTryRead_CP(theStrimU, '"'))
					{
					fQuotesSeen = 2;
					}
				else
					{
					size_t countRead;
					ZStrimR_Escaped(theStrimU, '"')
						.Read(localDest, localDestEnd - localDest, &countRead);
					localDest += countRead;

					if (sTryRead_CP(theStrimU, '"'))
						fQuotesSeen = 0;
					else if (countRead == 0)
						spThrowParseException("Expected \" to close a string");
					}
				break;
				}
			case 2:
				{
				if (sTryRead_CP(theStrimU, '"'))
					{
					fQuotesSeen = 3;
					UTF32 theCP = theStrimU.ReadCP();
					if (not Unicode::sIsEOL(theCP))
						theStrimU.Unread(theCP);
					}
				else
					{
					// We have two quotes in a row, followed by something
					// else, so we had an empty string segment.
					fQuotesSeen = 0;
					}
				break;
				}
			case 3:
				{
				// We've got three quotes in a row, and any trailing EOL
				// has been stripped.
				size_t countRead;
				fStrimR_Boundary.Read(localDest, localDestEnd - localDest, &countRead);
				localDest += countRead;
				if (countRead == 0)
					{
					if (not fStrimR_Boundary.HitBoundary())
						spThrowParseException("Expected \"\"\" to close a string");
					fStrimR_Boundary.Reset();
					fQuotesSeen = 0;
					}
				break;
				}
			}
		}

	if (oCount)
		*oCount = localDest - oDest;
	}

// =================================================================================================
#pragma mark -
#pragma mark ZYadSeqR_ZooLibStrim

ZYadSeqR_ZooLibStrim::ZYadSeqR_ZooLibStrim(ZRef<ZStrimmerU> iStrimmerU, bool iReadDelimiter)
:	fStrimmerU(iStrimmerU),
	fReadDelimiter(iReadDelimiter)
	{}

void ZYadSeqR_ZooLibStrim::Imp_ReadInc(bool iIsFirst, ZRef<ZYadR>& oYadR)
	{
	using namespace ZUtil_Strim;

	const ZStrimU& theStrimU = fStrimmerU->GetStrimU();

	sSkip_WSAndCPlusPlusComments(theStrimU);

	bool gotSeparator = true;
	if (not iIsFirst)
		{
		if (not sTryRead_CP(theStrimU, ',') && !sTryRead_CP(theStrimU, ';'))
			gotSeparator = false;
		else
			sSkip_WSAndCPlusPlusComments(theStrimU);
		}

	if (fReadDelimiter)
		{
		if (sTryRead_CP(theStrimU, ']'))
			return;
		}

	if (not gotSeparator)
		spThrowParseException("Expected ';' or ',' between values");

	if (not (oYadR = spMakeYadR_ZooLibStrim(fStrimmerU)))
		{
		if (not fReadDelimiter)
			return;
		spThrowParseException("Expected a value");
		}
	}

// =================================================================================================
#pragma mark -
#pragma mark ZYadMapR_ZooLibStrim

ZYadMapR_ZooLibStrim::ZYadMapR_ZooLibStrim(ZRef<ZStrimmerU> iStrimmerU, bool iReadDelimiter)
:	fStrimmerU(iStrimmerU),
	fReadDelimiter(iReadDelimiter)
	{}

void ZYadMapR_ZooLibStrim::Imp_ReadInc(bool iIsFirst, Name& oName, ZRef<ZYadR>& oYadR)
	{
	using namespace ZUtil_Strim;

	const ZStrimU& theStrimU = fStrimmerU->GetStrimU();

	sSkip_WSAndCPlusPlusComments(theStrimU);

	if (not iIsFirst)
		{
		if (not sTryRead_CP(theStrimU, ',') && !sTryRead_CP(theStrimU, ';'))
			spThrowParseException("Expected ';' or ',' after property");
		sSkip_WSAndCPlusPlusComments(theStrimU);
		}

	if (fReadDelimiter)
		{
		if (sTryRead_CP(theStrimU, '}'))
			return;
		}

	if (not spTryRead_PropertyName(theStrimU, oName))
		{
		if (not fReadDelimiter)
			return;
		spThrowParseException("Expected property name");
		}

	sSkip_WSAndCPlusPlusComments(theStrimU);

	if (not sTryRead_CP(theStrimU, '='))
		spThrowParseException("Expected '=' after property name");

	if (not (oYadR = spMakeYadR_ZooLibStrim(fStrimmerU)))
		spThrowParseException("Expected value after '='");
	}

// =================================================================================================
#pragma mark -
#pragma mark Static writing functions

static void spWriteIndent(const ChanW_UTF& iStrimW,
	size_t iCount, const ZYadOptions& iOptions)
	{
	while (iCount--)
		iStrimW << iOptions.fIndentString;
	}

static void spWriteLFIndent(const ChanW_UTF& iStrimW,
	size_t iCount, const ZYadOptions& iOptions)
	{
	iStrimW << iOptions.fEOLString;
	spWriteIndent(iStrimW, iCount, iOptions);
	}

static void spWriteString(
	const ChanW_UTF& s, const ZYadOptions& iOptions, const string& iString)
	{
	if (iOptions.fBreakStrings
		&& iOptions.DoIndentation()
		&& string::npos != iString.find_first_of("\n\r"))
		{
		// We put a newline after the opening """, which will be
		// ignored by sFromStrim, so the first line of iString
		// will be in column zero.
		s << "\"\"\"\n";
		ZStrimU_String strim_String(iString);
		ZStrimR_Boundary strim_Boundary("\"\"\"", strim_String);
		for (;;)
			{
			s << strim_Boundary;
			if (not strim_Boundary.HitBoundary())
				{
				// We've returned without having hit the boundary, so we're done.
				break;
				}
			strim_Boundary.Reset();

			// Close the triple quotes.
			s << "\"\"\"";
			// A space to separate the triple-quote from the single quote
			s << " ";
			// An open quote
			s << "\"";
			// Three escaped quotes.
			s << "\\\"\\\"\\\"";
			// A close quote.
			s << "\"";
			// Another space, for symmetry
			s << " ";
			// And re-open triple quotes again.
			s << "\"\"\"";
			// With a newline, so the text will again
			// start in column zero.
			s << "\n";
			}
		s << "\"\"\"";
		}
	else
		{
		string delimiter = "\"";
		bool quoteQuotes = true;
		if (string::npos != iString.find('"') && string::npos == iString.find('\''))
			{
			delimiter = "'";
			quoteQuotes = false;
			}

		s << delimiter;

		ZStrimW_Escaped::Options theOptions;
		theOptions.fQuoteQuotes = quoteQuotes;
		theOptions.fEscapeHighUnicode = false;

		ZStrimW_Escaped(theOptions, s) << iString;

		s << delimiter;
		}
	}

static void spToStrim_Stream(const ChanW_UTF& s, const ZStreamRPos& iStreamRPos,
	size_t iLevel, const ZYadOptions& iOptions, bool iMayNeedInitialLF)
	{
	uint64 theSize = iStreamRPos.GetSize();
	if (theSize == 0)
		{
		// we've got an empty Raw
		s << "()";
		}
	else
		{
		if (iOptions.DoIndentation() && theSize > iOptions.fRawChunkSize)
			{
			if (iMayNeedInitialLF)
				spWriteLFIndent(s, iLevel, iOptions);

			uint64 countRemaining = theSize;
			if (iOptions.fRawSizeCap)
				countRemaining = min(countRemaining, iOptions.fRawSizeCap.Get());

			sWritefMust(s, "( // %lld bytes", theSize);

			if (countRemaining < theSize)
				sWritefMust(s, " (truncated at %lld bytes)", iOptions.fRawSizeCap.Get());

			spWriteLFIndent(s, iLevel, iOptions);
			if (iOptions.fRawAsASCII)
				{
				for (;;)
					{
					uint64 lastPos = iStreamRPos.GetPosition();
					const size_t countToCopy =
						min(iOptions.fRawChunkSize, ZStream::sClampedSize(countRemaining));
					uint64 countCopied;
					ZStreamW_HexStrim(iOptions.fRawByteSeparator, string(), 0, s)
						.CopyFrom(iStreamRPos, countToCopy, &countCopied, nullptr);
					countRemaining -= countCopied;
					if (countCopied == 0)
						break;

					iStreamRPos.SetPosition(lastPos);
					if (size_t extraSpaces = iOptions.fRawChunkSize - countCopied)
						{
						// We didn't write a complete line of bytes, so pad it out.
						while (extraSpaces--)
							{
							// Two spaces for the two nibbles
							s << "  ";
							// And then the separator sequence
							s << iOptions.fRawByteSeparator;
							}
						}

					s << " // ";
					while (countCopied--)
						{
						UTF32 theChar = iStreamRPos.ReadInt8();
						if (theChar < 0x20 || theChar > 0x7E)
							sWriteMust(UTF32('.'), s);
						else
							sWriteMust(theChar, s);
						}
					spWriteLFIndent(s, iLevel, iOptions);
					}
				}
			else
				{
				string eol = iOptions.fEOLString;
				for (size_t xx = 0; xx < iLevel; ++xx)
					eol += iOptions.fIndentString;

				ZStreamW_HexStrim(iOptions.fRawByteSeparator,
					eol, iOptions.fRawChunkSize, s).CopyAllFrom(iStreamRPos);

				spWriteLFIndent(s, iLevel, iOptions);
				}

			s << ")";
			}
		else
			{
			s << "(";

			ZStreamW_HexStrim(iOptions.fRawByteSeparator, string(), 0, s)
				.CopyAllFrom(iStreamRPos);

			if (iOptions.fRawAsASCII)
				{
				iStreamRPos.SetPosition(0);
				s << " /* ";
				while (theSize--)
					{
					UTF32 theChar = iStreamRPos.ReadInt8();
					if (theChar < 0x20 || theChar > 0x7E)
						sWriteMust(UTF32('.'), s);
					else
						sWriteMust(theChar, s);
					}
				s << " */";
				}
			s << ")";
			}
		}
	}

static void spToStrim_Stream(const ChanW_UTF& s, const ZStreamR& iStreamR,
	size_t iLevel, const ZYadOptions& iOptions, bool iMayNeedInitialLF)
	{
	if (const ZStreamRPos* theStreamRPos = dynamic_cast<const ZStreamRPos*>(&iStreamR))
		{
		spToStrim_Stream(s, *theStreamRPos, iLevel, iOptions, iMayNeedInitialLF);
		}
	else
		{
		s << "(";

		ZStreamW_HexStrim(iOptions.fRawByteSeparator, string(), 0, s)
			.CopyAllFrom(iStreamR);

		s << ")";
		}
	}

static void spToStrim_Strim(const ChanW_UTF& s, const ZStrimR& iStrimR,
	size_t iLevel, const ZYadOptions& iOptions, bool iMayNeedInitialLF)
	{
	if (const ZStrimU_String8* theStrimU = dynamic_cast<const ZStrimU_String8*>(&iStrimR))
		{
		spWriteString(s, iOptions, theStrimU->GetString8());
		}
	else
		{
		s << "\"";

		ZStrimW_Escaped::Options theOptions;
		theOptions.fQuoteQuotes = true;
		theOptions.fEscapeHighUnicode = false;

		ZStrimW_Escaped(theOptions, s) << iStrimR;

		s << "\"";
		}
	}

static void spToStrim_SimpleValue(const ChanW_UTF& s, const ZAny& iVal,
	size_t iLevel, const ZYadOptions& iOptions, bool iMayNeedInitialLF)
	{
	if (false)
		{}
	else if (iVal.Type() == typeid(void))
		{
		s << "Null";
		}
#if 0
	else if (const ZType* theValue = iVal.PGet<ZType>())
		{
		s << "Type(" << ZTypeAsString(*theValue) << ")";
		}
#endif
	else if (const uint64* theValue = iVal.PGet<uint64>())
		{
		sWritefMust(s, "ID(0x%0llX)", *theValue);
		}
	else if (const char* theValue = iVal.PGet<char>())
		{
		sWritefMust(s, "int8(%d)", *theValue);
		}
	else if (const unsigned char* theValue = iVal.PGet<unsigned char>())
		{
		sWritefMust(s, "int8(%d)", *theValue);
		}
	else if (const signed char* theValue = iVal.PGet<signed char>())
		{
		sWritefMust(s, "int8(%d)", *theValue);
		}
	else if (const short* theValue = iVal.PGet<short>())
		{
		sWritefMust(s, "int16(%d)", *theValue);
		}
	else if (const unsigned short* theValue = iVal.PGet<unsigned short>())
		{
		sWritefMust(s, "int16(%d)", *theValue);
		}
	else if (const int* theValue = iVal.PGet<int>())
		{
		if (ZIntIs32Bit)
			sWritefMust(s, "int32(%d)", *theValue);
		else
			sWritefMust(s, "int64(%lld)", int64(*theValue));
		}
	else if (const unsigned int* theValue = iVal.PGet<unsigned int>())
		{
		if (ZIntIs32Bit)
			sWritefMust(s, "int32(%d)", *theValue);
		else
			sWritefMust(s, "int64(%lld)", int64(*theValue));
		}
	else if (const long* theValue = iVal.PGet<long>())
		{
		if (ZLongIs32Bit)
			sWritefMust(s, "int32(%ld)", *theValue);
		else
			sWritefMust(s, "int64(%lld)", int64(*theValue));
		}
	else if (const unsigned long* theValue = iVal.PGet<unsigned long>())
		{
		if (ZLongIs32Bit)
			sWritefMust(s, "int32(%ld)", long(*theValue));
		else
			sWritefMust(s, "int64(%lld)", int64(*theValue));
		}
	else if (const int64* theValue = iVal.PGet<int64>())
		{
		sWritefMust(s, "int64(%lld)", *theValue);
		}
	else if (const bool* theValue = iVal.PGet<bool>())
		{
		if (*theValue)
			s << "true";
		else
			s << "false";
		}
	else if (const float* theValue = iVal.PGet<float>())
		{
		s << "float(";
		ZUtil_Strim::sWriteExact(*theValue, s);
		s << ")";
		}
	else if (const double* theValue = iVal.PGet<double>())
		{
		s << "double(";
		ZUtil_Strim::sWriteExact(*theValue, s);
		s << ")";
		}
	else if (const ZTime* theValue = iVal.PGet<ZTime>())
		{
		if (*theValue)
			{
			s << "time(";
			s << Util_Time::sAsString_ISO8601(*theValue, true);
			s << ")";
			}
		else
			{
			// We're now allowing empty parens to represent invalid times.
			s << "time()";
			}
		}
	else if (const VoidStar_t* theValue = iVal.PGet<VoidStar_t>())
		{
		sWritefMust(s, "pointer(%p)", *theValue);
		}
	else if (const ZRectPOD* theValue = iVal.PGet<ZRectPOD>())
		{
		sWritefMust(s, "Rect(%d, %d, %d, %d)",
			theValue->left,
			theValue->top,
			theValue->right,
			theValue->bottom);
		}
	else if (const ZPointPOD* theValue = iVal.PGet<ZPointPOD>())
		{
		sWritefMust(s, "Point(%d, %d)",
			theValue->h,
			theValue->v);
		}
	else if (const ZRef<ZCounted>* theValue =
		iVal.PGet<ZRef<ZCounted> >())
		{
		sWritefMust(s, "RefCounted(%p)", theValue->Get());
		}
	else
		{
		s << "null /*Unhandled: " << iVal.Type().name() << "*/";
		}
	}

static void spToStrim_Yad(const ChanW_UTF& s, ZRef<ZYadR> iYadR,
	size_t iInitialIndent, const ZYadOptions& iOptions, bool iMayNeedInitialLF);

static void spToStrim_Seq(const ChanW_UTF& s, ZRef<ZYadSeqR> iYadSeqR,
	size_t iLevel, const ZYadOptions& iOptions, bool iMayNeedInitialLF)
	{
	bool needsIndentation = false;
	if (iOptions.DoIndentation())
		{
		// We're supposed to be indenting if we're complex, ie if any element is:
		// 1. A non-empty vector.
		// 2. A non-empty tuple.
		// or if iOptions.fBreakStrings is true, any element is a string with embedded
		// line breaks or more than iOptions.fStringLineLength characters.
		needsIndentation = !iYadSeqR->IsSimple(iOptions);
		}

	if (needsIndentation)
		{
		// We need to indent.
		if (iMayNeedInitialLF)
			{
			// We were invoked by a tuple which has already issued the property
			// name and equals sign, so we need to start a fresh line.
			spWriteLFIndent(s, iLevel, iOptions);
			}

		s << "[";
		for (bool isFirst = true; /*no test*/ ; isFirst = false)
			{
			if (ZRef<ZYadR,false> cur = iYadSeqR->ReadInc())
				{
				break;
				}
			else
				{
				if (not isFirst)
					s << ", ";
				spWriteLFIndent(s, iLevel, iOptions);
				spToStrim_Yad(s, cur, iLevel, iOptions, false);
				}
			}
		spWriteLFIndent(s, iLevel, iOptions);
		s << "]";
		}
	else
		{
		// We're not indenting, so we can just dump everything out on
		// one line, with just some spaces to keep things legible.
		s << "[";
		for (bool isFirst = true; /*no test*/ ; isFirst = false)
			{
			if (ZRef<ZYadR,false> cur = iYadSeqR->ReadInc())
				{
				break;
				}
			else
				{
				if (not isFirst)
					s << ", ";
				spToStrim_Yad(s, cur, iLevel, iOptions, false);
				}
			}
		s << "]";
		}
	}

static void spToStrim_Map(const ChanW_UTF& s, ZRef<ZYadMapR> iYadMapR,
	size_t iLevel, const ZYadOptions& iOptions, bool iMayNeedInitialLF)
	{
	bool needsIndentation = false;
	if (iOptions.DoIndentation())
		{
		needsIndentation = ! iYadMapR->IsSimple(iOptions);
		}

	if (needsIndentation)
		{
		if (iMayNeedInitialLF)
			{
			// We're going to be indenting, but need to start
			// a fresh line to have our { and contents line up.
			spWriteLFIndent(s, iLevel, iOptions);
			}

		s << "{";
		for (;;)
			{
			Name curName;
			if (ZRef<ZYadR,false> cur = iYadMapR->ReadInc(curName))
				{
				break;
				}
			else
				{
				spWriteLFIndent(s, iLevel, iOptions);
				ZYad_ZooLibStrim::sWrite_PropName(curName, s);
				s << " = ";
				spToStrim_Yad(s, cur, iLevel + 1, iOptions, true);
				s << ";";
				}
			}
		spWriteLFIndent(s, iLevel, iOptions);
		s << "}";
		}
	else
		{
		s << "{";
		for (;;)
			{
			Name curName;
			if (ZRef<ZYadR,false> cur = iYadMapR->ReadInc(curName))
				{
				break;
				}
			else
				{
				s << " ";
				ZYad_ZooLibStrim::sWrite_PropName(curName, s);
				s << " = ";
				spToStrim_Yad(s, cur, iLevel + 1, iOptions, true);
				s << ";";
				}
			}
		s << " }";
		}
	}

static void spToStrim_Yad(const ChanW_UTF& s, ZRef<ZYadR> iYadR,
	size_t iLevel, const ZYadOptions& iOptions, bool iMayNeedInitialLF)
	{
	if (not iYadR)
		{
		return;
		}
	else if (ZRef<ZYadMapR> theYadMapR = iYadR.DynamicCast<ZYadMapR>())
		{
		spToStrim_Map(s, theYadMapR, iLevel, iOptions, iMayNeedInitialLF);
		}
	else if (ZRef<ZYadSeqR> theYadSeqR = iYadR.DynamicCast<ZYadSeqR>())
		{
		spToStrim_Seq(s, theYadSeqR, iLevel, iOptions, iMayNeedInitialLF);
		}
	else if (ZRef<ZYadStreamerR> theYadStreamerR = iYadR.DynamicCast<ZYadStreamerR>())
		{
		spToStrim_Stream(s, theYadStreamerR->GetStreamR(), iLevel, iOptions, iMayNeedInitialLF);
		}
	else if (ZRef<ZYadStrimmerR> theYadStrimmerR = iYadR.DynamicCast<ZYadStrimmerR>())
		{
		spToStrim_Strim(s, theYadStrimmerR->GetStrimR(), iLevel, iOptions, iMayNeedInitialLF);
		}
	else if (ZRef<ZYadAtomR> theYadAtomR = iYadR.DynamicCast<ZYadAtomR>())
		{
		spToStrim_SimpleValue(s, theYadAtomR->AsAny(),
			iLevel, iOptions, iMayNeedInitialLF);
		}
	else
		{
		s << "null /*!! Unhandled yad !!*/";
		}
	}

// =================================================================================================
#pragma mark -
#pragma mark ZYad_ZooLibStrim

bool ZYad_ZooLibStrim::sRead_Identifier(
	const ZStrimU& iStrimU, string* oStringLC, string* oStringExact)
	{
	if (oStringExact)
		oStringExact->reserve(32);

	if (oStringLC)
		oStringLC->reserve(32);

	bool gotAny = false;
	for (;;)
		{
		UTF32 theCP;
		if (not iStrimU.ReadCP(theCP))
			break;
		if (not Unicode::sIsAlphaDigit(theCP) && theCP != '_')
			{
			iStrimU.Unread(theCP);
			break;
			}

		gotAny = true;

		if (oStringLC)
			*oStringLC += Unicode::sToLower(theCP);
		if (oStringExact)
			*oStringExact += theCP;
		}
	return gotAny;
	}

ZRef<ZYadR> ZYad_ZooLibStrim::sYadR(ZRef<ZStrimmerU> iStrimmerU)
	{ return spMakeYadR_ZooLibStrim(iStrimmerU); }

void ZYad_ZooLibStrim::sToStrim(ZRef<ZYadR> iYadR, const ChanW_UTF& s)
	{ spToStrim_Yad(s, iYadR, 0, ZYadOptions(), false); }

void ZYad_ZooLibStrim::sToStrim(size_t iInitialIndent, const ZYadOptions& iOptions,
	ZRef<ZYadR> iYadR, const ChanW_UTF& s)
	{ spToStrim_Yad(s, iYadR, iInitialIndent, iOptions, false); }

static bool spContainsProblemChars(const string& iString)
	{
	if (iString.empty())
		{
		// An empty string can't be distinguished from no string at all, so
		// we treat it as if it has problem chars, so it will be wrapped in quotes.
		return true;
		}

	for (string::const_iterator ii = iString.begin(), end = iString.end();;)
		{
		UTF32 theCP;
		if (not Unicode::sReadInc(ii, end, theCP))
			break;
		if (not Unicode::sIsAlphaDigit(theCP) && '_' != theCP)
			return true;
		}

	return false;
	}

void ZYad_ZooLibStrim::sWrite_PropName(const string& iPropName, const ChanW_UTF& iStrimW)
	{
	if (spContainsProblemChars(iPropName))
		{
		iStrimW << "\"";
		ZStrimW_Escaped(iStrimW) << iPropName;
		iStrimW << "\"";
		}
	else
		{
		iStrimW << iPropName;
		}
	}

} // namespace ZooLib
