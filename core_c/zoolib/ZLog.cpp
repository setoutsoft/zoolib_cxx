/* -------------------------------------------------------------------------------------------------
Copyright (c) 2005 Andrew Green and Learning in Motion, Inc.
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

#include "zoolib/ZCompat_string.h"
#include "zoolib/ZLog.h"
#include "zoolib/ZStringf.h"
#include "zoolib/ZThread.h"
#include "zoolib/ZUtil_string.h"

using std::string;

namespace ZooLib {
namespace ZLog {

ZRef<LogMeister> sLogMeister;
//ZSafe<ZRef<LogMeister> > sLogMeister;

// =================================================================================================
// MARK: - ZLog::CallDepth

static ZTSS::Key spKey()
	{
	static ZAtomicPtr_t spKey;
	return ZTSS::sKey(spKey);
	}

CallDepth::CallDepth(bool iActive)
:	fActive(iActive)
,	fPrior(((CallDepth*)(ZTSS::sGet(spKey()))))
	{ ZTSS::sSet(spKey(), this); }

CallDepth::~CallDepth()
	{ ZTSS::sSet(spKey(), const_cast<CallDepth*>(fPrior)); }

size_t CallDepth::sCount()
	{
	size_t count = 0;
	for (const CallDepth* current = ((CallDepth*)(ZTSS::sGet(spKey())));
		current; current = current->fPrior)
		{ ++count; }
	return count;
	}

size_t CallDepth::sCountActive()
	{
	size_t count = 0;
	for (const CallDepth* current = ((CallDepth*)(ZTSS::sGet(spKey())));
		current; current = current->fPrior)
		{
		if (current->fActive)
			++count;
		}
	return count;
	}

// =================================================================================================
// MARK: - String/integer mapping

static const char* const spNames[] =
	{
	"Emerg",
	"Alert",
	"Crit",
	"Err",
	"Warning",
	"Notice",
	"Info",
	"Debug"
	};

EPriority sPriorityFromName(const string& iString)
	{
	for (int priority = 0; priority <= eDebug; ++priority)
		{
		if (0 == ZUtil_string::sEquali(iString, spNames[priority]))
			return priority;
		}
	return -1;
	}

string sNameFromPriority(EPriority iPriority)
	{
	if (iPriority < 0)
		return sStringf("%d", iPriority);

	if (iPriority <= eDebug)
		return spNames[iPriority];

	return spNames[eDebug] + sStringf("+%d", iPriority - eDebug);
	}

// =================================================================================================
// MARK: - ZLog::StrimW

StrimW::StrimW(EPriority iPriority, const string& iName_String)
:	fPriority(iPriority)
,	fName_StringQ(iName_String)
,	fOutdent(false)
	{}

StrimW::StrimW(EPriority iPriority, const char* iName_CharStar)
:	fPriority(iPriority)
,	fName_CharStarQ(iName_CharStar)
,	fOutdent(false)
	{}

StrimW::~StrimW()
	{
	if (fMessageQ && !fMessageQ->empty())
		this->pEmit();
	}

void StrimW::Imp_WriteUTF32(const UTF32* iSource, size_t iCountCU, size_t* oCountCU)
	{
	try
		{
		ZStrimW_String8(sMut(fMessageQ)).Write(iSource, iCountCU, oCountCU);
		}
	catch (...)
		{
		if (oCountCU)
			*oCountCU = 0;
		}
	}

void StrimW::Imp_WriteUTF16(const UTF16* iSource, size_t iCountCU, size_t* oCountCU)
	{
	try
		{
		ZStrimW_String8(sMut(fMessageQ)).Write(iSource, iCountCU, oCountCU);
		}
	catch (...)
		{
		if (oCountCU)
			*oCountCU = 0;
		}
	}

void StrimW::Imp_WriteUTF8(const UTF8* iSource, size_t iCountCU, size_t* oCountCU)
	{
	try
		{
		ZStrimW_String8(sMut(fMessageQ)).Write(iSource, iCountCU, oCountCU);
		}
	catch (...)
		{
		if (oCountCU)
			*oCountCU = 0;
		}
	}

StrimW::operator operator_bool() const
	{
	if (ZRef<LogMeister> theLM = sLogMeister)
		{
		if (fName_StringQ)
			{
			return operator_bool_gen::translate(
				theLM->Enabled(fPriority, *fName_StringQ));
			}
		else
			{
			return operator_bool_gen::translate(
				theLM->Enabled(fPriority, *fName_CharStarQ));
			}
		}
	else
		{
		return operator_bool_gen::translate(false);
		}
	}

void StrimW::Emit() const
	{
	if (fMessageQ && !fMessageQ->empty())
		{
		const_cast<StrimW*>(this)->pEmit();
		sMut(fMessageQ).resize(0);
		}
	}

void StrimW::pEmit()
	{
	if (not fName_StringQ)
		fName_StringQ = *fName_CharStarQ;
	size_t theDepth = CallDepth::sCountActive();
	if (fOutdent and theDepth > 0)
		theDepth -= 1;
	sLogIt(fPriority, *fName_StringQ, theDepth, *fMessageQ);
	}

// =================================================================================================
// MARK: - ZLog::LogMeister

bool LogMeister::Enabled(EPriority iPriority, const string& iName)
	{ return true; }

bool LogMeister::Enabled(EPriority iPriority, const char* iName)
	{ return true; }

void sLogIt(EPriority iPriority, const std::string& iName, size_t iDepth, const std::string& iMessage)
	{
	if (ZRef<LogMeister> theLM = sLogMeister)
		{
		try
			{
			theLM->LogIt(iPriority, iName, iDepth, iMessage);
			}
		catch (...)
			{}
		}
	}

// =================================================================================================
// MARK: - ZLog::FunctionEntryExit

FunctionEntryExit::FunctionEntryExit(EPriority iPriority, const char* iFunctionName, const std::string& iMessage)
:	fPriority(iPriority)
,	fFunctionName(iFunctionName)
	{
	if (const S& s = S(fPriority, "ZLF"))
		{
		s.fOutdent = true;
		s << "> " << fFunctionName << iMessage;
		}
	}


FunctionEntryExit::FunctionEntryExit(EPriority iPriority, const char* iFunctionName)
:	fPriority(iPriority)
,	fFunctionName(iFunctionName)
	{
	if (const S& s = S(fPriority, "ZLF"))
		{
		s.fOutdent = true;
		s << "> " << fFunctionName;
		}
	}

FunctionEntryExit::~FunctionEntryExit()
	{
	if (const S& s = S(fPriority, "ZLF"))
		{
		s.fOutdent = true;
		s << "< " << fFunctionName;
		}
	}

// =================================================================================================
// MARK: - ZLog::sLogTrace

static const char* spTruncateFileName(const char* iFilename)
	{
	#if ZCONFIG_SPI_Enabled(Win)
		if (const char* truncatedFilename = strrchr(iFilename, '\\'))
		return truncatedFilename + 1;
	#else
		if (const char* truncatedFilename = strrchr(iFilename, '/'))
		return truncatedFilename + 1;
	#endif

	return iFilename;
	}

void sLogTrace(EPriority iPriority, const char* iFile, int iLine, const char* iFunctionName)
	{
	if (const S& s = S(iPriority, "ZLOGTRACE"))
		s << spTruncateFileName(iFile) << ":" << sStringf("%d", iLine) << ", in " << iFunctionName;
	}

} // namespace ZLog
} // namespace ZooLib