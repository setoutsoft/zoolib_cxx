/* -------------------------------------------------------------------------------------------------
Copyright (c) 2003 Andrew Green and Learning in Motion, Inc.
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

#ifndef __ZUtil_Strim_Tuple__
#define __ZUtil_Strim_Tuple__ 1
#include "zconfig.h"

#include "ZTypes.h" // for size_t
#include "zoolib/ZYAD_ZooLib.h"

#include <string>
#include <vector>

class ZStrimR;
class ZStrimU;
class ZStrimW;

class ZTName;
class ZTuple;
class ZTValue;

namespace ZUtil_Strim_Tuple {

// =================================================================================================
#pragma mark -
#pragma mark * ZUtil_Strim_Tuple::Options

typedef ZYADOptions Options;

// =================================================================================================
#pragma mark -
#pragma mark * ZUtil_Strim_Tuple::Format

struct Format
	{
	Format(const ZTValue& iTValue, int iInitialIndent, const ZYADOptions& iOptions);

	const ZTValue& fTValue;
	int fInitialIndent;
	const ZYADOptions& fOptions;
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZUtil_Strim_Tuple, writing and parsing pieces

void sWrite_PropName(const ZStrimW& iStrimW, const ZTName& iPropName);

bool sRead_Identifier(const ZStrimU& iStrimU, std::string* oStringLC, std::string* oStringExact);

// =================================================================================================
#pragma mark -
#pragma mark * ZUtil_Strim_Tuple, TValue

void sToStrim(const ZStrimW& iStrimW, const ZTValue& iTValue);

void sToStrim(const ZStrimW& iStrimW, const ZTValue& iTValue,
	size_t iInitialIndent, const ZYADOptions& iOptions);

std::string sAsString(const ZTValue& iTValue);

bool sFromStrim(const ZStrimU& iStrimU, ZTValue& oTValue);

bool sFromString(const std::string& iString, ZTValue& oTValue);

// =================================================================================================
#pragma mark -
#pragma mark * ZUtil_Strim_Tuple, vector<ZTValue>

void sToStrim(const ZStrimW& iStrimW, const std::vector<ZTValue>& iVector);

void sToStrim(const ZStrimW& iStrimW, const std::vector<ZTValue>& iVector,
	size_t iInitialIndent, const ZYADOptions& iOptions);

std::string sAsString(const std::vector<ZTValue>& iVector);

bool sFromStrim(const ZStrimU& iStrimU, std::vector<ZTValue>& oVector);

bool sFromString(const std::string& iString, std::vector<ZTValue>& oVector);

// =================================================================================================
#pragma mark -
#pragma mark * ZUtil_Strim_Tuple, ZTuple

void sToStrim(const ZStrimW& iStrimW, const ZTuple& iTuple);

void sToStrim(const ZStrimW& iStrimW, const ZTuple& iTuple,
	size_t iInitialIndent, const ZYADOptions& iOptions);

std::string sAsString(const ZTuple& iTuple);

std::string sAsString(const ZTuple& iTuple,
	size_t iInitialIndent, const ZYADOptions& iOptions);

bool sFromStrim(const ZStrimU& iStrimU, ZTuple& oTuple);

bool sFromString(const std::string& iString, ZTuple& oTuple);

}  // namespace ZUtil_Strim_Tuple

// =================================================================================================
#pragma mark -
#pragma mark * operator<< overloads

const ZStrimW& operator<<(const ZStrimW& s, const ZTValue& iTValue);

const ZStrimW& operator<<(const ZStrimW& s, const ZUtil_Strim_Tuple::Format& iFormat);

#endif // __ZUtil_Strim_Tuple__
