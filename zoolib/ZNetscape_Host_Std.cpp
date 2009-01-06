/* -------------------------------------------------------------------------------------------------
Copyright (c) 2002 Andrew Green
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

#include "zoolib/ZNetscape_Host_Std.h"

#if ZCONFIG_SPI_Enabled(Netscape)

#include "zoolib/ZDebug.h"
#include "zoolib/ZHTTP_Requests.h"
#include "zoolib/ZLog.h"
#include "zoolib/ZMIME.h"
#include "zoolib/ZNet_Internet.h"
#include "zoolib/ZNetscape.h"
#include "zoolib/ZStreamRWPos_RAM.h"
#include "zoolib/ZStream_CRLF.h"
#include "zoolib/ZStream_Tee.h"
#include "zoolib/ZString.h"
#include "zoolib/ZThreadSimple.h"
#include "zoolib/ZTuple.h"
#include "zoolib/ZUtil_STL.h"
#include "zoolib/ZUtil_Strim_Tuple.h"

#include "zoolib/ZCompat_string.h" // For strdup

#ifdef NPVERS_HAS_RESPONSE_HEADERS
	typedef NPStream NPStream_Z;
#else
	struct NPStream_Z : public NPStream
		{
		const char* headers;
		};
#endif

#include <vector>

NAMESPACE_ZOOLIB_BEGIN

using std::list;
using std::set;
using std::string;
using std::vector;

namespace ZNetscape {

// =================================================================================================
#pragma mark -
#pragma mark * HostMeister_Std

HostMeister_Std::HostMeister_Std()
	{}

HostMeister_Std::~HostMeister_Std()
	{}

Host_Std* HostMeister_Std::sHostFromNPP_Std(NPP npp)
	{ return static_cast<Host_Std*>(sHostFromNPP(npp)); }

Host_Std* HostMeister_Std::sHostFromStream_Std(NPStream* iNPStream)
	{ return static_cast<Host_Std*>(sHostFromStream(iNPStream)); }

NPError HostMeister_Std::GetURL(NPP npp, const char* URL, const char* window)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "GetURL";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		return theHost->Host_GetURL(npp, URL, window);

	return NPERR_GENERIC_ERROR;
	}

NPError HostMeister_Std::PostURL(NPP npp,
	const char* URL, const char* window, uint32 len, const char* buf, NPBool file)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "PostURL";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		return theHost->Host_PostURL(npp, URL, window, len, buf, file);

	return NPERR_GENERIC_ERROR;
	}

NPError HostMeister_Std::RequestRead(NPStream* stream, NPByteRange* rangeList)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "RequestRead";

	if (Host_Std* theHost = sHostFromStream_Std(stream))
		return theHost->Host_RequestRead(stream, rangeList);

	return NPERR_GENERIC_ERROR;
	}

NPError HostMeister_Std::NewStream(NPP npp,
	NPMIMEType type, const char* window, NPStream** stream)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "NewStream";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		return theHost->Host_NewStream(npp, type, window, stream);

	return NPERR_GENERIC_ERROR;
	}

int32 HostMeister_Std::Write(NPP npp, NPStream* stream, int32 len, void* buffer)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "Write";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		return theHost->Host_Write(npp, stream, len, buffer);

	return -1;
	}

NPError HostMeister_Std::DestroyStream(NPP npp, NPStream* stream, NPReason reason)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "DestroyStream";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		return theHost->Host_DestroyStream(npp, stream, reason);

	return NPERR_GENERIC_ERROR;
	}

void HostMeister_Std::Status(NPP npp, const char* message)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "Status";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		theHost->Host_Status(npp, message);
	}

const char* HostMeister_Std::UserAgent(NPP npp)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "UserAgent";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		return theHost->Host_UserAgent(npp);

	return nil;
	}

void* HostMeister_Std::MemAlloc(uint32 size)
	{ return ::malloc(size); }

void HostMeister_Std::MemFree(void* ptr)
	{ ::free(ptr); }

uint32 HostMeister_Std::MemFlush(uint32 size)
	{ return 0; }

void HostMeister_Std::ReloadPlugins(NPBool reloadPages)
	{}

void* HostMeister_Std::GetJavaEnv()
	{ return nil; }

void* HostMeister_Std::GetJavaPeer(NPP npp)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "GetJavaPeer";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		return theHost->Host_GetJavaPeer(npp);

	return nil;
	}

NPError HostMeister_Std::GetURLNotify(NPP npp,
	const char* URL, const char* window, void* notifyData)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "GetURLNotify";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		return theHost->Host_GetURLNotify(npp, URL, window, notifyData);

	return NPERR_GENERIC_ERROR;
	}

NPError HostMeister_Std::PostURLNotify(NPP npp,
	const char* URL, const char* window,
	uint32 len, const char* buf, NPBool file, void* notifyData)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "PostURLNotify";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		return theHost->Host_PostURLNotify(npp, URL, window, len, buf, file, notifyData);

	return NPERR_GENERIC_ERROR;
	}

NPError HostMeister_Std::GetValue(NPP npp, NPNVariable variable, void* ret_value)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "GetValue";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		return theHost->Host_GetValue(npp, variable, ret_value);

	return NPERR_GENERIC_ERROR;
	}

NPError HostMeister_Std::SetValue(NPP npp, NPPVariable variable, void* value)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "SetValue";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		return theHost->Host_SetValue(npp, variable, value);

	return NPERR_GENERIC_ERROR;
	}

void HostMeister_Std::InvalidateRect(NPP npp, NPRect* rect)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "InvalidateRect";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		theHost->Host_InvalidateRect(npp, rect);
	}

void HostMeister_Std::InvalidateRegion(NPP npp, NPRegion region)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "InvalidateRegion";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		theHost->Host_InvalidateRegion(npp, region);
	}

void HostMeister_Std::ForceRedraw(NPP npp)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "ForceRedraw";

	if (Host_Std* theHost = sHostFromNPP_Std(npp))
		return theHost->Host_ForceRedraw(npp);
	}

NPIdentifier HostMeister_Std::GetStringIdentifier(const NPUTF8* name)
	{
	const string theName = name;
	set<string>::iterator iter = fStrings.lower_bound(theName);
	if (iter == fStrings.end() || *iter != theName)
		iter = fStrings.insert(iter, theName);
	return static_cast<NPIdentifier>(const_cast<string*>(&*iter));
	}

void HostMeister_Std::GetStringIdentifiers(
	const NPUTF8* *names, int32_t nameCount, NPIdentifier* identifiers)
	{
	while (--nameCount)
		*identifiers++ = this->GetStringIdentifier(*names++);
	}

NPIdentifier HostMeister_Std::GetIntIdentifier(int32_t intid)
	{ return reinterpret_cast<NPIdentifier>((intid << 1) | 0x1); }

bool HostMeister_Std::IdentifierIsString(NPIdentifier identifier)
	{ return 0 == (reinterpret_cast<intptr_t>(identifier) & 0x1); }

NPUTF8* HostMeister_Std::UTF8FromIdentifier(NPIdentifier identifier)
	{
	if (0 == (reinterpret_cast<intptr_t>(identifier) & 0x1))
		return (NPUTF8*)strdup(static_cast<string*>(identifier)->c_str());
	return nil;
	}

int32_t HostMeister_Std::IntFromIdentifier(NPIdentifier identifier)
	{
	const int32_t asInt = reinterpret_cast<intptr_t>(identifier);
	if (asInt & 0x1)
		return asInt >> 1;
	return 0;
	}

NPObject* HostMeister_Std::CreateObject(NPP npp, NPClass* aClass)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s.Writef("CreateObject");

	ZAssert(aClass);
	NPObject* result;
	if (aClass->allocate)
		result = aClass->allocate(npp, aClass);
	else
		result = (NPObject*)malloc(sizeof(NPObject));

	result->_class = aClass;
	result->referenceCount = 1;

	return result;
	}

NPObject* HostMeister_Std::RetainObject(NPObject* obj)
	{
	ZAssert(obj);
	++obj->referenceCount;
	return obj;
	}

void HostMeister_Std::ReleaseObject(NPObject* obj)
	{
	ZAssert(obj && obj->referenceCount > 0);
	if (--obj->referenceCount == 0)
		{
		if (obj->_class->deallocate)
			obj->_class->deallocate(obj);
		else
			free(obj);
		}
	}

bool HostMeister_Std::Invoke(NPP npp,
	NPObject* obj, NPIdentifier methodName, const NPVariant* args, uint32_t argCount,
	NPVariant* result)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "Invoke: " << this->StringFromIdentifier(methodName);

	if (obj && obj->_class && obj->_class->invoke)
		return obj->_class->invoke(obj, methodName, args, argCount, result);

	return false;
	}

bool HostMeister_Std::InvokeDefault(NPP npp,
	NPObject* obj, const NPVariant* args, uint32_t argCount, NPVariant* result)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s.Writef("InvokeDefault");

	if (obj && obj->_class && obj->_class->invokeDefault)
		return obj->_class->invokeDefault(obj, args, argCount, result);

	return false;
	}

bool HostMeister_Std::Evaluate(NPP npp,
	NPObject* obj, NPString* script, NPVariant* result)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s.Writef("Evaluate");
	return false;
	}

bool HostMeister_Std::GetProperty(NPP npp,
	NPObject* obj, NPIdentifier propertyName, NPVariant* result)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s << "GetProperty: " << this->StringFromIdentifier(propertyName);

	if (obj && obj->_class && obj->_class->getProperty)
		return obj->_class->getProperty(obj, propertyName, result);

	return false;
	}

bool HostMeister_Std::SetProperty(NPP npp,
	NPObject* obj, NPIdentifier propertyName, const NPVariant* value)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s.Writef("SetProperty");

	if (obj && obj->_class && obj->_class->getProperty)
		return obj->_class->setProperty(obj, propertyName, value);

	return false;
	}

bool HostMeister_Std::RemoveProperty(NPP npp, NPObject* obj, NPIdentifier propertyName)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s.Writef("RemoveProperty");

	if (obj && obj->_class && obj->_class->removeProperty)
		return obj->_class->removeProperty(obj, propertyName);

	return false;
	}

bool HostMeister_Std::HasProperty(NPP, NPObject* obj, NPIdentifier propertyName)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s.Writef("HasProperty");

	if (obj && obj->_class && obj->_class->hasProperty)
		return obj->_class->hasProperty(obj, propertyName);

	return false;
	}

bool HostMeister_Std::HasMethod(NPP npp, NPObject* obj, NPIdentifier methodName)
	{
	if (ZLOG(s, eDebug, "HostMeister_Std"))
		s.Writef("HasMethod");

	if (obj && obj->_class && obj->_class->hasMethod)
		return obj->_class->hasMethod(obj, methodName);

	return false;
	}

void HostMeister_Std::ReleaseVariantValue(NPVariant* variant)
	{
	ZAssert(variant);

	if (variant->type == NPVariantType_Object)
		{
		this->ReleaseObject(variant->value.objectValue);
		variant->value.objectValue = nil;
		}
	else if (variant->type == NPVariantType_String)
		{
		free((void*)sNPStringChars(variant->value.stringValue));
		sNPStringChars(variant->value.stringValue) = nil;
		sNPStringLength(variant->value.stringValue) = 0;
		}
	variant->type = NPVariantType_Void;
	}

void HostMeister_Std::SetException(NPObject* obj, const NPUTF8* message)
	{
	ZUnimplemented();
	}

// =================================================================================================
#pragma mark -
#pragma mark * Host_Std::HTTPer

class Host_Std::HTTPer
	{
public:
	HTTPer(Host_Std* iHost, const string& iURL, ZMemoryBlock* iMB, void* iNotifyData);

	void Start();
	void Cancel();

private:
	void pRun();
	static void spRun(HTTPer* iHTTPer);

	const string fURL;
	void* fNotifyData;
	Host_Std* fHost;
	ZMemoryBlock fMB;
	bool fIsPOST;
	};

Host_Std::HTTPer::HTTPer(Host_Std* iHost, const string& iURL, ZMemoryBlock* iMB, void* iNotifyData)
:	fHost(iHost),
	fURL(iURL),
	fNotifyData(iNotifyData)
	{
	if (iMB)
		{
		fMB = *iMB;
		fIsPOST = true;
		}
	else
		{
		fIsPOST = false;
		}
	}

void Host_Std::HTTPer::Start()
	{ (new ZThreadSimple<HTTPer*>(spRun, this))->Start(); }

void Host_Std::HTTPer::Cancel()
	{
	fHost = nil;
	}

void Host_Std::HTTPer::pRun()
	{
	try
		{
		string theURL = fURL;
		ZTuple theHeaders;
		ZMemoryBlock theRawHeaders;	
		ZRef<ZStreamerR> theStreamerR;
		if (fIsPOST)
			{
			theStreamerR = ZHTTP::sPost(
				theURL, ZStreamRWPos_MemoryBlock(fMB), nil, &theHeaders, &theRawHeaders);
			}
		else
			{
			theStreamerR = ZHTTP::sRequest("GET", theURL, nil, &theHeaders, &theRawHeaders);
			}

		if (theStreamerR && fHost)
			{
			const ZTuple theCT = theHeaders.GetTuple("content-type");
			const string theMIME = theCT.GetString("type") + "/" + theCT.GetString("subtype");

			fHost->pHTTPerFinished(
				this, fNotifyData, theURL, theMIME, theRawHeaders, theStreamerR);
			return;
			}
		}
	catch (...)
		{}

	// This causes async delivery of an error.
	if (fHost)
		{
		fHost->pHTTPerFinished(
			this, fNotifyData, fURL, "", ZMemoryBlock(), ZRef<ZStreamerR>());
		}
	}

void Host_Std::HTTPer::spRun(HTTPer* iHTTPer)
	{
	iHTTPer->pRun();
	delete iHTTPer;
	}

// =================================================================================================
#pragma mark -
#pragma mark * Host_Std::Sender

class Host_Std::Sender
	{
public:
	Sender(Host* iHost, const NPP_t& iNPP_t,
		void* iNotifyData,
		const std::string& iURL, const std::string& iMIME, const ZMemoryBlock& iHeaders,
		ZRef<ZStreamerR> iStreamerR);
	~Sender();

	bool DeliverData();

private:
	bool pDeliverData();

	bool fSentNew;
	Host* fHost;
	void* fNotifyData;
	const string fURL;
	const string fMIME;
	const ZMemoryBlock fHeaders;
	NPStream_Z fNPStream;
	ZRef<ZStreamerR> fStreamerR;
	};

Host_Std::Sender::Sender(Host* iHost, const NPP_t& iNPP_t,
	void* iNotifyData,
	const std::string& iURL, const std::string& iMIME, const ZMemoryBlock& iHeaders,
	ZRef<ZStreamerR> iStreamerR)
:	fSentNew(false),
	fHost(iHost),
	fNotifyData(iNotifyData),
	fURL(iURL),
	fMIME(iMIME),
	fHeaders(iHeaders),
	fStreamerR(iStreamerR)
	{
	fNPStream.ndata = iNPP_t.ndata;
	fNPStream.pdata = iNPP_t.pdata;
	fNPStream.url = fURL.c_str();
	fNPStream.end = 0;
	fNPStream.lastmodified = 0;
	fNPStream.notifyData = iNotifyData;
	fNPStream.headers = static_cast<const char*>(fHeaders.GetData());
	}

Host_Std::Sender::~Sender()
	{}

bool Host_Std::Sender::DeliverData()
	{
	if (!fSentNew)
		{
		fSentNew = true;

		if (!fStreamerR)
			{
			fHost->Guest_URLNotify(fURL.c_str(), NPRES_NETWORK_ERR, fNotifyData);
			return false;
			}

		uint16 theStreamType = NP_NORMAL;
		if (fHost->Guest_NewStream(
			const_cast<char*>(fMIME.c_str()), &fNPStream, false, &theStreamType))
			{
			// Failed -- what result should we pass?
			fHost->Guest_URLNotify(fURL.c_str(), NPRES_NETWORK_ERR, fNotifyData);
			return false;
			}
		}


	if (this->pDeliverData())
		return true;

	fHost->Guest_URLNotify(fURL.c_str(), NPRES_DONE, fNotifyData);

	fHost->Guest_DestroyStream(&fNPStream, NPRES_DONE);

	return false;
	}

bool Host_Std::Sender::pDeliverData()
	{
	const ZStreamR& theStreamR = fStreamerR->GetStreamR();

	if (!theStreamR.WaitReadable(0))
		{
		if (ZLOG(s, eDebug + 1, "Host_Std::Sender"))
			s.Writef("waitReadable is false");
		return true;
		}

	const size_t countReadable = theStreamR.CountReadable();

	if (ZLOG(s, eDebug + 1, "Host_Std::Sender"))
		s.Writef("countReadable = %d", countReadable);
	
	if (countReadable == 0)
		return false;

	int32 countPossible = fHost->Guest_WriteReady(&fNPStream);

	if (ZLOG(s, eDebug + 1, "Host_Std::Sender"))
		s.Writef("countPossible = %d", countPossible);

	if (countPossible < 0)
		{
		return false;
		}
	else if (countPossible > 0)
		{
		countPossible = std::min(countPossible, int32(64 * 1024));

		vector<uint8> buffer;
		buffer.resize(countPossible);
		size_t countRead;
		theStreamR.Read(&buffer[0], countPossible, &countRead);
		if (countRead == 0)
			return false;

		if (ZLOG(s, eDebug + 1, "Host_Std::Sender"))
			s.Writef("countRead = %d", countRead);

		for (size_t start = 0; start < countRead; /*no inc*/)
			{
			int countWritten = fHost->Guest_Write(&fNPStream, 0, countRead - start, &buffer[start]);

			if (ZLOG(s, eDebug + 1, "Host_Std::Sender"))
				s.Writef("countWritten = %d", countWritten);

			if (countWritten < 0)
				return false;

			start += countWritten;
			}
		}

	return true;
	}

// =================================================================================================
#pragma mark -
#pragma mark * Host_Std

Host_Std::Host_Std(ZRef<GuestFactory> iGuestFactory)
:	Host(iGuestFactory)
	{
	#if defined(XP_MAC) || defined(XP_MACOSX)
		ZBlockZero(&fNP_Port, sizeof(fNP_Port));
	#endif
	}

Host_Std::~Host_Std()
	{
	ZMutexLocker locker(fMutex);
	for (vector<HTTPer*>::iterator i = fHTTPers.begin(); i != fHTTPers.end(); ++i)
		{
		(*i)->Cancel();
		}
	}

NPError Host_Std::Host_GetURL(NPP npp, const char* URL, const char* window)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s << "GetURL: " << URL;
	return NPERR_INVALID_URL;
	}

NPError Host_Std::Host_PostURL(NPP npp,
	const char* URL, const char* window, uint32 len, const char* buf, NPBool file)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s << "PostURL: " << URL;
	return NPERR_INVALID_URL;
	}

NPError Host_Std::Host_RequestRead(NPStream* stream, NPByteRange* rangeList)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s.Writef("RequestRead");
	return NPERR_GENERIC_ERROR;
	}

NPError Host_Std::Host_NewStream(NPP npp,
	NPMIMEType type, const char* window, NPStream** stream)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s.Writef("NewStream");
	return NPERR_GENERIC_ERROR;
	}

int32 Host_Std::Host_Write(NPP npp, NPStream* stream, int32 len, void* buffer)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s.Writef("Write");
	return -1;
	}

NPError Host_Std::Host_DestroyStream(NPP npp, NPStream* stream, NPReason reason)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s.Writef("DestroyStream");
	return NPERR_GENERIC_ERROR;
	}

void Host_Std::Host_Status(NPP npp, const char* message)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s.Writef("Status");
	}

const char* Host_Std::Host_UserAgent(NPP npp)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s.Writef("UserAgent");

	return "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)";
	}

void* Host_Std::Host_GetJavaPeer(NPP npp)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s.Writef("GetJavaPeer");
	return nil;
	}

NPError Host_Std::Host_GetURLNotify(NPP npp,
	const char* URL, const char* window, void* notifyData)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		{
		s << "GetURLNotify: " << URL;
		if (window)
			s << " target: " << window;
		}

	if (URL == strstr(URL, "http:"))
		{
		HTTPer* theG = new HTTPer(this, URL, nil, notifyData);

		ZMutexLocker locker(fMutex);
		fHTTPers.push_back(theG);
		theG->Start();
		return NPERR_NO_ERROR;
		}

	return NPERR_INVALID_URL;
	}

NPError Host_Std::Host_PostURLNotify(NPP npp,
	const char* URL, const char* window,
	uint32 len, const char* buf, NPBool file, void* notifyData)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s << "PostURLNotify: " << URL;

	if (URL == strstr(URL, "http:"))
		{
		ZMemoryBlock theData(buf, len);
		HTTPer* theG = new HTTPer(this, URL, &theData, notifyData);

		ZMutexLocker locker(fMutex);
		fHTTPers.push_back(theG);
		theG->Start();
		return NPERR_NO_ERROR;
		}

	return NPERR_INVALID_URL;
	}

NPError Host_Std::Host_GetValue(NPP npp, NPNVariable variable, void* ret_value)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s << "GetValue, " << ZNetscape::sAsString(variable) ;
	return NPERR_GENERIC_ERROR;
	}

NPError Host_Std::Host_SetValue(NPP npp, NPPVariable variable, void* value)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s << "SetValue, " << ZNetscape::sAsString(variable) ;
	return NPERR_GENERIC_ERROR;
	}

void Host_Std::Host_InvalidateRect(NPP npp, NPRect* rect)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s.Writef("InvalidateRect");
	}

void Host_Std::Host_InvalidateRegion(NPP npp, NPRegion region)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s.Writef("InvalidateRegion");
	}

void Host_Std::Host_ForceRedraw(NPP npp)
	{
	if (ZLOG(s, eDebug, "Host_Std"))
		s.Writef("ForceRedraw");
	}

void Host_Std::pHTTPerFinished(HTTPer* iHTTPer, void* iNotifyData,
	const std::string& iURL, const std::string& iMIME, const ZMemoryBlock& iHeaders,
	ZRef<ZStreamerR> iStreamerR)
	{
	ZMutexLocker locker(fMutex);
	ZUtil_STL::sEraseMustContain(1, fHTTPers, iHTTPer);
	this->SendDataAsync(iNotifyData, iURL, iMIME, iHeaders, iStreamerR);
	}

void Host_Std::Create(const string& iURL, const string& iMIME)
	{
    char* npp_argv[] =
		{ const_cast<char*>(iURL.c_str()), const_cast<char*>(iMIME.c_str()),};

    char* npp_argn[] =
		{ "src", "type" };

	NPError theErr = this->Guest_New(
		const_cast<char*>(iMIME.c_str()), NP_FULL,
		countof(npp_argn), npp_argn, npp_argv, nil);

	if (ZLOG(s, eDebug + 1, "Host"))
		s.Writef("Create, theErr: %d", theErr);
	}

void Host_Std::Destroy()
	{
	NPSavedData* theSavedData;
	this->Guest_Destroy(&theSavedData);
	}

void Host_Std::SendDataAsync(
	void* iNotifyData,
	const std::string& iURL, const std::string& iMIME, const ZMemoryBlock& iHeaders,
	ZRef<ZStreamerR> iStreamerR)
	{
	ZMutexLocker locker(fMutex);
	Sender* theSender = new Sender(this, this->GetNPP(),
		iNotifyData, iURL, iMIME, iHeaders, iStreamerR);
	fSenders.push_back(theSender);
	}

void Host_Std::SendDataSync(
	void* iNotifyData,
	const string& iURL, const string& iMIME,
	const ZStreamR& iStreamR)
	{
	NPStream_Z theNPStream;
	theNPStream.ndata = this->GetNPP().ndata;
	theNPStream.pdata = this->GetNPP().pdata;
	theNPStream.url = iURL.c_str();
	theNPStream.end = iStreamR.CountReadable();
	theNPStream.lastmodified = 0;
	theNPStream.notifyData = iNotifyData;
	theNPStream.headers = nil;

	uint16 theStreamType = NP_NORMAL;

	if (0 == this->Guest_NewStream(
		const_cast<char*>(iMIME.c_str()), &theNPStream, false, &theStreamType))
		{
		for (bool keepGoing = true; keepGoing; /*no inc*/)
			{
			int32 countPossible = this->Guest_WriteReady(&theNPStream);
			if (countPossible < 0)
				{
				// Failure.
				break;
				}

			countPossible = std::min(countPossible, int32(1024 * 1024));

			vector<uint8> buffer;
			buffer.resize(countPossible);
			size_t countRead;
			iStreamR.Read(&buffer[0], countPossible, &countRead);
			if (countRead == 0)
				break;

			for (size_t start = 0; start < countRead; /*no inc*/)
				{
				int countWritten = this->Guest_Write(
					&theNPStream, 0, countRead - start, &buffer[start]);

				if (countWritten < 0)
					{
					if (ZLOG(s, eDebug, "Host"))
						{
						s << "write failure";
						}
					keepGoing = false;
					break;
					}
				start += countWritten;
				}
			}

		if (iNotifyData)
            this->Guest_URLNotify(iURL.c_str(), NPRES_DONE, iNotifyData);

		this->Guest_DestroyStream(&theNPStream, NPRES_DONE);
		}
	}

void Host_Std::DeliverData()
	{
	ZMutexLocker locker(fMutex);
	for (list<Sender*>::iterator i = fSenders.begin(); i != fSenders.end(); /*no inc*/)
		{
		if ((*i)->DeliverData())
			{
			++i;
			}
		else
			{
			delete *i;
			i = fSenders.erase(i);
			}
		}
	}

NPObjectH* Host_Std::CopyScriptableNPObject()
	{
	NPObjectH* theNPObject;
	this->Guest_GetValue(NPPVpluginScriptableNPObject, &theNPObject);
	return theNPObject;
	}

#if defined(XP_MAC) || defined(XP_MACOSX)

static void sStuffEventRecord(EventRecord& oEventRecord)
	{
	oEventRecord.what = nullEvent;
	oEventRecord.message = 0;
	oEventRecord.when = ::TickCount();
	::GetGlobalMouse(&oEventRecord.where);
	oEventRecord.modifiers = 0;
	}

void Host_Std::DoActivate(bool iActivate)
	{
	if (fNP_Port.port)
		{
		EventRecord theER;
		sStuffEventRecord(theER);

		theER.what = activateEvt;
		theER.message = (UInt32)::GetWindowFromPort(fNP_Port.port);
		if (iActivate)
			theER.modifiers = activeFlag;
		else
			theER.modifiers = 0;
		
		this->DoEvent(theER);
		}
	}

void Host_Std::DoFocus(bool iFocused)
	{
	EventRecord theER;
	sStuffEventRecord(theER);

	if (iFocused)
		theER.what = NPEventType_GetFocusEvent;
	else
		theER.what = NPEventType_LoseFocusEvent;

	this->DoEvent(theER);
	}

void Host_Std::DoIdle()
	{
	EventRecord theER;
	sStuffEventRecord(theER);

	this->DoEvent(theER);
	}

void Host_Std::DoDraw()
	{
	if (fNP_Port.port)
		{
		EventRecord theER;
		sStuffEventRecord(theER);

		theER.what = updateEvt;
		theER.message = (UInt32)::GetWindowFromPort(fNP_Port.port);

		this->DoEvent(theER);
		}
	}

void Host_Std::SetPortAndBounds(CGrafPtr iGrafPtr,
	ZPoint iLocation, ZPoint iSize, const ZRect& iClip)
	{
	fNP_Port.port = iGrafPtr;
	this->SetBounds(iLocation, iSize, iClip);
	}

void Host_Std::SetBounds(
	ZPoint iLocation, ZPoint iSize, const ZRect& iClip)
	{
	fNP_Port.portx = -iLocation.h;
	fNP_Port.porty = -iLocation.v;
	fNPWindow.window = &fNP_Port;

	fNPWindow.type = NPWindowTypeDrawable;

	fNPWindow.x = iLocation.h;
	fNPWindow.y = iLocation.v;
	fNPWindow.width = iSize.h;
	fNPWindow.height = iSize.v;

	fNPWindow.clipRect.left = iClip.left;
	fNPWindow.clipRect.top = iClip.top;
	fNPWindow.clipRect.right = iClip.right;
	fNPWindow.clipRect.bottom = iClip.bottom;

	this->Guest_SetWindow(&fNPWindow);
	}

void Host_Std::DoEvent(const EventRecord& iEvent)
	{
	// Hmm -- do we need to use the local?
	EventRecord localEvent = iEvent;
	this->Guest_HandleEvent(&localEvent);
	}

#endif // defined(XP_MAC) || defined(XP_MACOSX)

} // namespace ZNetscape

NAMESPACE_ZOOLIB_END

#endif // ZCONFIG_SPI_Enabled(Netscape)
