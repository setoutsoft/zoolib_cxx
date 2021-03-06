/* -------------------------------------------------------------------------------------------------
Copyright (c) 2002 Andrew Green and Learning in Motion, Inc.
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

#ifndef __ZDCPixmapCoder_h__
#define __ZDCPixmapCoder_h__ 1
#include "zconfig.h"

#include "zoolib/ChanR_Bin.h"
#include "zoolib/ChanW_Bin.h"

#include "zoolib/ZDCPixmap.h"

namespace ZooLib {

// =================================================================================================
// MARK: - ZDCPixmapEncoder

class ZDCPixmapEncoder
	{
public:
	static void sWritePixmap(const ChanW_Bin& iStream,
		const ZDCPixmap& iPixmap, ZDCPixmapEncoder& iEncoder);

protected:
	ZDCPixmapEncoder();
	ZDCPixmapEncoder(const ZDCPixmapEncoder&);
	ZDCPixmapEncoder& operator=(const ZDCPixmapEncoder&);

public:
	virtual ~ZDCPixmapEncoder();

	void Write(const ChanW_Bin& iStream, const ZDCPixmap& iPixmap);
	void Write(const ChanW_Bin& iStream,
		const void* iBaseAddress,
		const ZDCPixmapNS::RasterDesc& iRasterDesc,
		const ZDCPixmapNS::PixelDesc& iPixelDesc,
		const ZRectPOD& iBounds);

	/** API that must be overridden. */
	virtual void Imp_Write(const ChanW_Bin& iStream,
		const void* iBaseAddress,
		const ZDCPixmapNS::RasterDesc& iRasterDesc,
		const ZDCPixmapNS::PixelDesc& iPixelDesc,
		const ZRectPOD& iBounds) = 0;
	};

// =================================================================================================
// MARK: - ZDCPixmapDecoder

class ZDCPixmapDecoder
	{
public:
	static ZDCPixmap sReadPixmap(const ChanR_Bin& iStream, ZDCPixmapDecoder& iDecoder);

protected:
	ZDCPixmapDecoder();
	ZDCPixmapDecoder(const ZDCPixmapDecoder&);
	ZDCPixmapDecoder& operator=(const ZDCPixmapDecoder&);

public:
	virtual ~ZDCPixmapDecoder();

	ZDCPixmap Read(const ChanR_Bin& iStream);

	/** API that must be overridden. */
	virtual void Imp_Read(const ChanR_Bin& iStream, ZDCPixmap& oPixmap) = 0;

	/** \todo. Need an API that reads into a subset only. */
	};

} // namespace ZooLib

#endif // __ZDCPixmapCoder_h__
