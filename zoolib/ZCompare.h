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

#ifndef __ZCompare__
#define __ZCompare__ 1
#include "zconfig.h"

#include "zoolib/ZCompare_T.h"

#include <typeinfo>

NAMESPACE_ZOOLIB_BEGIN

// =================================================================================================
#pragma mark -
#pragma mark * ZCompare

class ZCompare
	{
protected:
	ZCompare(const char* iTypeName);
	~ZCompare();

	virtual int Compare(const void* iL, const void* iR) = 0;

public:
	static int sCompare(const char* iTypeName, const void* iL, const void* iR);
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZCompareRegistration_T

template <class T, int (*CompareProc)(const T& iL, const T& iR)>
class ZCompareRegistration_T : public ZCompare
	{
public:
	ZCompareRegistration_T() : ZCompare(typeid(T).name()) {}

// From ZCompare
	virtual int Compare(const void* iL, const void* iR)
		{ return CompareProc(*static_cast<const T*>(iL), *static_cast<const T*>(iR)); }
	};

// =================================================================================================
#pragma mark -
#pragma mark * Macros

#define ZMACRO_CompareRegistration_T_Real(t, CLASS, INST) \
	static class CLASS : public ZCompareRegistration_T<t, sCompare_T<t> > {} INST

#define ZMACRO_CompareRegistration_T(t) \
	ZMACRO_CompareRegistration_T_Real(t, \
	ZMACRO_CONCAT(ZComparer_,__LINE__), \
	ZMACRO_CONCAT(sComparer_,__LINE__))

NAMESPACE_ZOOLIB_END

#endif // __ZCompare__
