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

#ifndef __ZooLib_POSIX_Net_Socket_h__
#define __ZooLib_POSIX_Net_Socket_h__ 1
#include "zconfig.h"
#include "zoolib/ZCONFIG_API.h"
#include "zoolib/ZCONFIG_SPI.h"

#ifndef ZCONFIG_API_Avail__Net_Socket
	#define ZCONFIG_API_Avail__Net_Socket ZCONFIG_SPI_Enabled(POSIX)
#endif

#ifndef ZCONFIG_API_Desired__Net_Socket
	#define ZCONFIG_API_Desired__Net_Socket 1
#endif

#include "zoolib/Net.h"
#include "zoolib/ChannerXX.h"

#if ZCONFIG_API_Enabled(Net_Socket)

#include "zoolib/ZThread.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -
#pragma mark Net_Socket

class Net_Socket
	{
public:
	static int sSend(int iSocket, const char* iSource, size_t iCount);
	static int sReceive(int iSocket, char* oDest, size_t iCount);
	static bool sWaitReadable(int iSocket, double iTimeout);
	static void sWaitWriteable(int iSocket);
	static Net::Error sTranslateError(int iNativeError);
	};

// =================================================================================================
#pragma mark -
#pragma mark NetListener_Socket

class NetListener_Socket
:	public virtual NetListener
	{
protected:
	NetListener_Socket(int iSocketFD);
	virtual ~NetListener_Socket();

// From Factory_ChannerRWClose_Bin via NetListener
	virtual ZRef<ChannerRWClose_Bin> Make();
	virtual void Cancel();

// Our protocol
	int GetSocketFD();

	static void sEnableFastCancellation();

protected:
	ZMtx fMtx;
	ZCnd fCnd;
	int fSocketFD;
	ZThread::ID fThreadID_Listening;
	};

ZRef<NetListener_Socket> sNetListener_Socket(int iFD);

// =================================================================================================
#pragma mark -
#pragma mark NetEndpoint_Socket

class NetEndpoint_Socket
:	public ChannerRWClose_Bin
,	private ChanR_Bin
,	private ChanW_Bin
,	private ChanClose
	{
public:
	NetEndpoint_Socket(int iSocketFD);
	virtual ~NetEndpoint_Socket();

// From ChannerR_Bin
	virtual void GetChan(const ChanR_Bin*& oChanPtr);

// From ChannerW_Bin
	virtual void GetChan(const ChanW_Bin*& oChanPtr);

// From ChannerClose
	virtual void GetChan(const ChanClose*& oChanPtr);

// From ChanR_Bin
	virtual size_t QRead(byte* oDest, size_t iCount);
	virtual size_t Readable();

// From ChanW_Bin
	virtual size_t QWrite(const byte* iSource, size_t iCount);

// From ChanClose
	virtual void Close();

// Our protocol
	int GetSocketFD();

private:
	int fSocketFD;
	};

// =================================================================================================

} // namespace ZooLib

#endif // ZCONFIG_API_Enabled(Net_Socket)

#endif // __ZooLib_POSIX_Net_Socket_h__
