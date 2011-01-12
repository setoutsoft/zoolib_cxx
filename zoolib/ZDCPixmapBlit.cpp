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

#include "zoolib/ZDCPixmapBlit.h"

#if ZCONFIG(Compiler, CodeWarrior)
// Enforce higher level of optimization for this code.
#	pragma push
#	pragma optimization_level 4
#	pragma opt_common_subs on
#	pragma opt_lifetimes on
#	pragma peephole on
#	if ZCONFIG(Processor, x86)
#		pragma register_coloring on
#	endif
#endif

#include "zoolib/ZDCPixmapBlitPriv.h"

using std::min;

namespace ZooLib {

typedef ZDCPixmapNS::PixelDescRep PDRep;
typedef ZDCPixmapNS::PixelDescRep_Indexed PDRep_Indexed;
typedef ZDCPixmapNS::PixelDescRep_Gray PDRep_Gray;
typedef ZDCPixmapNS::PixelDescRep_Color PDRep_Color;

// =================================================================================================
#pragma mark -
#pragma mark * Public API

void ZDCPixmapBlit::sBlit(
	const void* iSourceAddress, const RD& iSourceRD, const ZRectPOD& iSourceB, const PD& iSourcePD,
	ZPointPOD iSourceOrigin,
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD,
	EOp iOp)
	{
	sBlit_T(iSourceAddress, iSourceRD, iSourceB, iSourcePD,
		iSourceOrigin,
		iDestAddress, iDestRD, iDestB, iDestPD,
		iOp);
	}

void ZDCPixmapBlit::sBlit(
	const void* iSourceAddress, const RD& iSourceRD, const ZRectPOD& iSourceB, const PD& iSourcePD,
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD,
	EOp iOp)
	{
	ZRectPOD realDest = iDestB;
	realDest.right = min(realDest.right, ZCoord(realDest.left + iSourceB.Width()));
	realDest.bottom = min(realDest.bottom, ZCoord(realDest.top + iSourceB.Height()));

	ZPointPOD sourceStart = iSourceB.TopLeft();
	sBlit_T(iSourceAddress, iSourceRD, iSourcePD,
		sourceStart,
		iDestAddress, iDestRD, realDest, iDestPD,
		iOp);

	}

void ZDCPixmapBlit::sBlit(
	const void* iSourceAddress, const RD& iSourceRD, const ZRectPOD& iSourceB, const PD& iSourcePD,
	ZPointPOD iSourceOrigin,
	const void* iMatteAddress, const RD& iMatteRD, const ZRectPOD& iMatteB, const PD& iMattePD,
	ZPointPOD iMatteOrigin,
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD,
	bool iSourcePremultiplied, EOp iOp)
	{
	sBlit_T(iSourceAddress, iSourceRD, iSourceB, iSourcePD,
		iSourceOrigin,
		iMatteAddress, iMatteRD, iMatteB, iMattePD,
		iMatteOrigin,
		iDestAddress, iDestRD, iDestB, iDestPD,
		iSourcePremultiplied, iOp);
	}

void ZDCPixmapBlit::sBlit(
	const void* iSourceAddress, const RD& iSourceRD, const ZRectPOD& iSourceB, const PD& iSourcePD,
	ZPointPOD iSourceOrigin,
	const void* iMatteAddress, const RD& iMatteRD, const ZRectPOD& iMatteB, const PD& iMattePD,
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD,
	bool iSourcePremultiplied, EOp iOp)
	{
	ZRectPOD realDest = iDestB;
	realDest.right = min(realDest.right, ZCoord(realDest.left + iMatteB.Width()));
	realDest.bottom = min(realDest.bottom, ZCoord(realDest.top + iMatteB.Height()));

	ZPointPOD matteStart = iMatteB.TopLeft();

	sBlit_T(iSourceAddress, iSourceRD, iSourceB, iSourcePD,
		iSourceOrigin,
		iMatteAddress, iMatteRD, iMattePD,
		matteStart,
		iDestAddress, iDestRD, realDest, iDestPD,
		iSourcePremultiplied, iOp);
	}

void ZDCPixmapBlit::sBlit(
	const void* iSourceAddress, const RD& iSourceRD, const ZRectPOD& iSourceB, const PD& iSourcePD,
	const void* iMatteAddress, const RD& iMatteRD, const ZRectPOD& iMatteB, const PD& iMattePD,
	ZPointPOD iMatteOrigin,
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD,
	bool iSourcePremultiplied, EOp iOp)
	{
	ZRectPOD realDest = iDestB;
	realDest.right = min(realDest.right, ZCoord(realDest.left + iSourceB.Width()));
	realDest.bottom = min(realDest.bottom, ZCoord(realDest.top + iSourceB.Height()));

	ZPointPOD sourceStart = iSourceB.TopLeft();

	ZRef<PDRep> sourcePDRep = iSourcePD.GetRep();
	ZRef<PDRep> mattePDRep = iMattePD.GetRep();
	ZRef<PDRep> destPDRep = iDestPD.GetRep();

	if (PDRep_Color* sourcePDRep_Color = sourcePDRep.DynamicCast<PDRep_Color>())
		{
		if (PDRep_Color* mattePDRep_Color = mattePDRep.DynamicCast<PDRep_Color>())
			{
			if (PDRep_Color* destPDRep_Color = destPDRep.DynamicCast<PDRep_Color>())
				{
				sBlit_T(iSourceAddress, iSourceRD, *sourcePDRep_Color,
					sourceStart,
					iMatteAddress, iMatteRD, iMatteB, *mattePDRep_Color,
					iMatteOrigin,
					iDestAddress, iDestRD, realDest, *destPDRep_Color,
					iSourcePremultiplied, iOp);
				return;
				}
			}
		}

	sBlit_T(iSourceAddress, iSourceRD, iSourcePD,
		sourceStart,
		iMatteAddress, iMatteRD, iMatteB, iMattePD,
		iMatteOrigin,
		iDestAddress, iDestRD, realDest, iDestPD,
		iSourcePremultiplied, iOp);
	}

void ZDCPixmapBlit::sBlit(
	const void* iSourceAddress, const RD& iSourceRD, const ZRectPOD& iSourceB, const PD& iSourcePD,
	const void* iMatteAddress, const RD& iMatteRD, const ZRectPOD& iMatteB, const PD& iMattePD,
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD,
	bool iSourcePremultiplied, EOp iOp)
	{
	ZRectPOD realDest = iDestB;
	int realWidth = min(iMatteB.Width(), iSourceB.Width());
	realDest.right = min(realDest.right, ZCoord(realDest.left + realWidth));

	int realHeight = min(iMatteB.Height(), iSourceB.Height());
	realDest.bottom = min(realDest.bottom, ZCoord(realDest.top + realHeight));

	ZPointPOD sourceStart = iSourceB.TopLeft();
	ZPointPOD matteStart = iMatteB.TopLeft();

	ZRef<PDRep> sourcePDRep = iSourcePD.GetRep();
	ZRef<PDRep> mattePDRep = iMattePD.GetRep();
	ZRef<PDRep> destPDRep = iDestPD.GetRep();

	if (PDRep_Color* sourcePDRep_Color = sourcePDRep.DynamicCast<PDRep_Color>())
		{
		if (PDRep_Color* mattePDRep_Color = mattePDRep.DynamicCast<PDRep_Color>())
			{
			if (PDRep_Color* destPDRep_Color = destPDRep.DynamicCast<PDRep_Color>())
				{
				sBlit_T(iSourceAddress, iSourceRD, *sourcePDRep_Color,
					sourceStart,
					iMatteAddress, iMatteRD, *mattePDRep_Color,
					matteStart,
					iDestAddress, iDestRD, realDest, *destPDRep_Color,
					iSourcePremultiplied, iOp);
				return;
				}
			}
		}
	sBlit_T(iSourceAddress, iSourceRD, iSourcePD, sourceStart,
			iMatteAddress, iMatteRD, iMattePD, matteStart,
			iDestAddress, iDestRD, realDest, iDestPD,
			iSourcePremultiplied, iOp);
	}

void ZDCPixmapBlit::sColor(
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD,
	const ZRGBA_POD& iColor,
	EOp iOp)
	{
	ZRef<PDRep> destPDRep = iDestPD.GetRep();
	if (PDRep_Color* destPDRep_Color = destPDRep.DynamicCast<PDRep_Color>())
		{
		sColor_T(iDestAddress, iDestRD, iDestB, *destPDRep_Color, iColor, iOp);
		}
	else if (PDRep_Indexed* destPDRep_Indexed = destPDRep.DynamicCast<PDRep_Indexed>())
		{
		sColor_T(iDestAddress, iDestRD, iDestB, *destPDRep_Indexed, iColor, iOp);
		}
	else if (PDRep_Gray* destPDRep_Gray = destPDRep.DynamicCast<PDRep_Gray>())
		{
		sColor_T(iDestAddress, iDestRD, iDestB, *destPDRep_Gray, iColor, iOp);
		}
	else
		{
		sColor_T(iDestAddress, iDestRD, iDestB, iDestPD, iColor, iOp);
		}
	}

void ZDCPixmapBlit::sColor(
	const void* iMatteAddress, const RD& iMatteRD, const ZRectPOD& iMatteB, const PD& iMattePD,
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD,
	const ZRGBA_POD& iColor,
	EOp iOp)
	{
	ZRectPOD realDest = iDestB;
	realDest.right = min(realDest.right, ZCoord(realDest.left + iMatteB.Width()));
	realDest.bottom = min(realDest.bottom, ZCoord(realDest.top + iMatteB.Height()));

	ZPointPOD matteStart = iMatteB.TopLeft();

	ZRef<PDRep> mattePDRep = iMattePD.GetRep();
	ZRef<PDRep> destPDRep = iDestPD.GetRep();

	if (PDRep_Color* mattePDRep_Color = mattePDRep.DynamicCast<PDRep_Color>())
		{
		if (PDRep_Color* destPDRep_Color = destPDRep.DynamicCast<PDRep_Color>())
			{
			sColor_T(iMatteAddress, iMatteRD, *mattePDRep_Color,
				matteStart,
				iDestAddress, iDestRD, realDest, *destPDRep_Color,
				iColor,
				iOp);
			return;
			}
		}
	else if (PDRep_Gray* mattePDRep_Gray = mattePDRep.DynamicCast<PDRep_Gray>())
		{
		if (PDRep_Color* destPDRep_Color = destPDRep.DynamicCast<PDRep_Color>())
			{
			sColor_T(iMatteAddress, iMatteRD, *mattePDRep_Gray,
				matteStart,
				iDestAddress, iDestRD, realDest, *destPDRep_Color,
				iColor,
				iOp);
			return;
			}
		}

	sColor_T(iMatteAddress, iMatteRD, iMattePD,
		matteStart,
		iDestAddress, iDestRD, realDest, iDestPD,
		iColor,
		iOp);
	}

void ZDCPixmapBlit::sColor(
	const void* iMatteAddress, const RD& iMatteRD, const ZRectPOD& iMatteB, const PD& iMattePD,
	ZPointPOD iMatteOrigin,
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD,
	const ZRGBA_POD& iColor,
	EOp iOp)
	{
	sColorTile_T(iMatteAddress, iMatteRD, iMatteB, iMattePD,
		iMatteOrigin,
		iDestAddress, iDestRD, iDestB, iDestPD,
		iColor,
		iOp);
	}

void ZDCPixmapBlit::sInvert(
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD)
	{
	ZRef<PDRep> destPDRep = iDestPD.GetRep();
	if (PDRep_Color* destPDRep_Color = destPDRep.DynamicCast<PDRep_Color>())
		{
		sInvert_T(iDestAddress, iDestRD, iDestB, *destPDRep_Color);
		}
	else if (PDRep_Indexed* destPDRep_Indexed = destPDRep.DynamicCast<PDRep_Indexed>())
		{
		sInvert_T(iDestAddress, iDestRD, iDestB, *destPDRep_Indexed);
		}
	else if (PDRep_Gray* destPDRep_Gray = destPDRep.DynamicCast<PDRep_Gray>())
		{
		sInvert_T(iDestAddress, iDestRD, iDestB, *destPDRep_Gray);
		}
	else
		{
		sInvert_T(iDestAddress, iDestRD, iDestB, iDestPD);
		}
	}

void ZDCPixmapBlit::sOpaque(
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD, uint16 iAmount)
	{
	ZRef<PDRep> destPDRep = iDestPD.GetRep();
	if (PDRep_Color* destPDRep_Color = destPDRep.DynamicCast<PDRep_Color>())
		{
		sOpaque_T(iDestAddress, iDestRD, iDestB, *destPDRep_Color, iAmount);
		}
	else if (PDRep_Indexed* destPDRep_Indexed = destPDRep.DynamicCast<PDRep_Indexed>())
		{
		sOpaque_T(iDestAddress, iDestRD, iDestB, *destPDRep_Indexed, iAmount);
		}
	else if (PDRep_Gray* destPDRep_Gray = destPDRep.DynamicCast<PDRep_Gray>())
		{
		sOpaque_T(iDestAddress, iDestRD, iDestB, *destPDRep_Gray, iAmount);
		}
	else
		{
		sOpaque_T(iDestAddress, iDestRD, iDestB, iDestPD, iAmount);
		}
	}

void ZDCPixmapBlit::sDarken(
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD, uint16 iAmount)
	{
	ZRef<PDRep> destPDRep = iDestPD.GetRep();
	if (PDRep_Color* destPDRep_Color = destPDRep.DynamicCast<PDRep_Color>())
		{
		sDarken_T(iDestAddress, iDestRD, iDestB, *destPDRep_Color, iAmount);
		}
	else if (PDRep_Indexed* destPDRep_Indexed = destPDRep.DynamicCast<PDRep_Indexed>())
		{
		sDarken_T(iDestAddress, iDestRD, iDestB, *destPDRep_Indexed, iAmount);
		}
	else if (PDRep_Gray* destPDRep_Gray = destPDRep.DynamicCast<PDRep_Gray>())
		{
		sDarken_T(iDestAddress, iDestRD, iDestB, *destPDRep_Gray, iAmount);
		}
	else
		{
		sDarken_T(iDestAddress, iDestRD, iDestB, iDestPD, iAmount);
		}
	}

void ZDCPixmapBlit::sFade(
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD, uint16 iAmount)
	{
	ZRef<PDRep> destPDRep = iDestPD.GetRep();
	if (PDRep_Color* destPDRep_Color = destPDRep.DynamicCast<PDRep_Color>())
		{
		sFade_T(iDestAddress, iDestRD, iDestB, *destPDRep_Color, iAmount);
		}
	else if (PDRep_Indexed* destPDRep_Indexed = destPDRep.DynamicCast<PDRep_Indexed>())
		{
		sFade_T(iDestAddress, iDestRD, iDestB, *destPDRep_Indexed, iAmount);
		}
	else if (PDRep_Gray* destPDRep_Gray = destPDRep.DynamicCast<PDRep_Gray>())
		{
		sFade_T(iDestAddress, iDestRD, iDestB, *destPDRep_Gray, iAmount);
		}
	else
		{
		sFade_T(iDestAddress, iDestRD, iDestB, iDestPD, iAmount);
		}
	}

void ZDCPixmapBlit::sApplyMatte(
	const void* iMatteAddress, const RD& iMatteRD, const ZRectPOD& iMatteB, const PD& iMattePD,
	void* iDestAddress, const RD& iDestRD, const ZRectPOD& iDestB, const PD& iDestPD)
	{
	ZAssertStop(1, iMattePD.HasAlpha());
	ZAssertStop(1, iDestPD.HasAlpha());
	ZRectPOD realDest = iDestB;
	realDest.right = min(realDest.right, ZCoord(realDest.left + iMatteB.Width()));
	realDest.bottom = min(realDest.bottom, ZCoord(realDest.top + iMatteB.Height()));

	ZPointPOD matteStart = iMatteB.TopLeft();

	ZRef<PDRep> mattePDRep = iMattePD.GetRep();
	ZRef<PDRep> destPDRep = iDestPD.GetRep();

	if (PDRep_Color* mattePDRep_Color = mattePDRep.DynamicCast<PDRep_Color>())
		{
		if (PDRep_Color* destPDRep_Color = destPDRep.DynamicCast<PDRep_Color>())
			{
			sApplyMatte_T(iMatteAddress, iMatteRD, *mattePDRep_Color,
				matteStart,
				iDestAddress, iDestRD, realDest, *destPDRep_Color);
			return;
			}
		}

	sApplyMatte_T(iMatteAddress, iMatteRD, iMattePD,
		matteStart,
		iDestAddress, iDestRD, iDestB, iDestPD);
	}

} // namespace ZooLib
