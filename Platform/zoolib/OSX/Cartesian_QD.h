/* -------------------------------------------------------------------------------------------------
Copyright (c) 2012 Andrew Green
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

#ifndef __ZooLib_OSX_Cartesian_QD_h__
#define __ZooLib_OSX_Cartesian_QD_h__ 1
#include "zconfig.h"
#include "zoolib/ZCONFIG_SPI.h"

#include "zoolib/Cartesian.h"

#if ZCONFIG_SPI_Enabled(QuickDraw)

#include ZMACINCLUDE3(CoreServices,CarbonCore,MacTypes.h)

namespace ZooLib {
namespace Cartesian {

// =================================================================================================
#pragma mark -
#pragma mark PointTraits<Point>

template <>
struct PointTraits<Point>
:	public PointTraits_Std_HV<short,Point,Rect>
	{
	static Point_t sMake(const Ord_t& iX, const Ord_t& iY)
		{
		const Point_t result = { iY, iX };
		return result;
		}
	};

// =================================================================================================
#pragma mark -
#pragma mark RectTraits<Rect>

template <>
struct RectTraits<Rect>
:	public RectTraits_Std_LeftTopRightBottom<short,Point,Rect>
	{
	static Rect_t sMake(const Ord_t& iL, const Ord_t& iT, const Ord_t& iR, const Ord_t& iB)
		{
		const Rect_t result = { iT, iL, iB, iR };
		return result;
		}
	};

// =================================================================================================
#pragma mark -
#pragma mark PointTraits<FixedPoint>

template <>
struct PointTraits<FixedPoint>
:	public PointTraits_Std_XY<Fixed,FixedPoint,FixedRect>
	{
	static Point_t sMake(const Ord_t& iX, const Ord_t& iY)
		{
		const Point_t result = { iX, iY };
		return result;
		}
	};

// =================================================================================================
#pragma mark -
#pragma mark RectTraits<FixedRect>

template <>
struct RectTraits<FixedRect>
:	public RectTraits_Std_LeftTopRightBottom<Fixed,FixedPoint,FixedRect>
	{
	static Rect_t sMake(const Ord_t& iL, const Ord_t& iT, const Ord_t& iR, const Ord_t& iB)
		{
		const Rect_t result = { iL, iT, iR, iB };
		return result;
		}
	};

} // namespace Cartesian
} // namespace ZooLib

#endif // ZCONFIG_SPI_Enabled(QuickDraw)
#endif // __ZooLib_OSX_Cartesian_QD_h__
