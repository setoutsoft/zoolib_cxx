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

#ifndef __ZooLib_Util_Strim_Cartesian_h__
#define __ZooLib_Util_Strim_Cartesian_h__ 1
#include "zconfig.h"

#include "zoolib/Cartesian.h"
#include "zoolib/Util_Chan_UTF_Operators.h"

namespace ZooLib {

template <class Point_p>
typename EnableIfC<Cartesian::PointTraits<Point_p>::value,const ChanW_UTF&>::type
operator<<(const ChanW_UTF& w, const Point_p& iPoint)
	{ return w << "(" << X(iPoint) << "," << Y(iPoint) << ")"; }

template <class Rect_p>
typename EnableIfC<Cartesian::RectTraits<Rect_p>::value,const ChanW_UTF&>::type
operator<<(const ChanW_UTF& w, const Rect_p& iRect)
	{ return w << "(" << L(iRect) << "," << T(iRect) << "," << R(iRect) << "," << B(iRect) << ")"; }

} // namespace ZooLib

#endif // __ZooLib_Util_Strim_Cartesian_h__
