/* -------------------------------------------------------------------------------------------------
Copyright (c) 2009 Andrew Green
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

#ifndef __ZVal_AppleEvent__
#define __ZVal_AppleEvent__ 1
#include "zconfig.h"
#include "zoolib/ZCONFIG_SPI.h"

#if ZCONFIG_SPI_Enabled(AppleEvent)

#include "zoolib/ZCompat_operator_bool.h"
#include "zoolib/ZUtil_AppleEvent.h"
#include "zoolib/ZVal.h"
#include "zoolib/ZValAccessors.h"

#include <string>

NAMESPACE_ZOOLIB_BEGIN

class ZVal_AppleEvent;
class ZValList_AppleEvent;
class ZValMap_AppleEvent;

typedef ZVal_AppleEvent ZVal_AE;
typedef ZValList_AppleEvent ZValList_AE;
typedef ZValMap_AppleEvent ZValMap_AE;

std::string sAEKeywordAsString(AEKeyword iKeyword);

// =================================================================================================
#pragma mark -
#pragma mark * ZVal_AppleEvent

class ZVal_AppleEvent
:	public AEDesc
,	public ZValR_T<ZVal_AppleEvent>
	{
	ZOOLIB_DEFINE_OPERATOR_BOOL_TYPES(ZVal_AppleEvent,
		operator_bool_generator_type, operator_bool_type);

public:
	operator operator_bool_type() const;

	void swap(ZVal_AppleEvent& iOther);

	ZVal_AppleEvent();
	ZVal_AppleEvent(const ZVal_AppleEvent& iOther);
	~ZVal_AppleEvent();
	ZVal_AppleEvent& operator=(const ZVal_AppleEvent& iOther);

	ZVal_AppleEvent(const AEDesc& iOther);
	ZVal_AppleEvent& operator=(const AEDesc& iOther);

	ZVal_AppleEvent(bool iVal);
	ZVal_AppleEvent(const std::string& iVal);
	ZVal_AppleEvent(const ZValList_AppleEvent& iVal);
	ZVal_AppleEvent(const ZValMap_AppleEvent& iVal);

	template <class T>
	ZVal_AppleEvent(const T& iVal)
		{
		const ZAEValRef_T<T> theValRef(iVal);
		::AECreateDesc(theValRef.sDescType, theValRef.Ptr(), theValRef.Size(), this);
		}

	template <class T, int D>
	explicit ZVal_AppleEvent(const ZAEValRef_T<T, D>& iValRef)
		{ ::AECreateDesc(iValRef.sDescType, iValRef.Ptr(), iValRef.Size(), this); }

	template <class T>
	ZVal_AppleEvent(DescType iDescType, const T& iVal)
		{ ::AECreateDesc(iDescType, &iVal, sizeof(iVal), this); }

	ZVal_AppleEvent(DescType iDescType, const void* iVal, size_t iSize)
		{ ::AECreateDesc(iDescType, iVal, iSize, this); }
	
// ZVal protocol
	void Clear();

	template <class S>
	bool QGet_T(S& oVal) const;

	template <class S>
	void Set_T(const S& iVal);

// Our protocol
	AEDesc& OParam();

// Typename accessors
	ZMACRO_ZValAccessors_Decl_Entry(ZVal_AppleEvent, Int16, int16)
	ZMACRO_ZValAccessors_Decl_Entry(ZVal_AppleEvent, Int32, int32)
	ZMACRO_ZValAccessors_Decl_Entry(ZVal_AppleEvent, Int64, int64)
	ZMACRO_ZValAccessors_Decl_Entry(ZVal_AppleEvent, Bool, bool)
	ZMACRO_ZValAccessors_Decl_Entry(ZVal_AppleEvent, Float, float)
	ZMACRO_ZValAccessors_Decl_Entry(ZVal_AppleEvent, Double, double)
	ZMACRO_ZValAccessors_Decl_Entry(ZVal_AppleEvent, String, std::string)
	ZMACRO_ZValAccessors_Decl_Entry(ZVal_AppleEvent, List, ZValList_AppleEvent)
	ZMACRO_ZValAccessors_Decl_Entry(ZVal_AppleEvent, Map, ZValMap_AppleEvent)
	ZMACRO_ZValAccessors_Decl_Entry(ZVal_AppleEvent, FSSpec, FSSpec)
	ZMACRO_ZValAccessors_Decl_Entry(ZVal_AppleEvent, FSRef, FSRef)
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZValList_AppleEvent

class ZValList_AppleEvent
:	public AEDescList
	{
	ZOOLIB_DEFINE_OPERATOR_BOOL_TYPES(ZValMap_AppleEvent,
		operator_bool_generator_type, operator_bool_type);

public:
	operator operator_bool_type() const;

	void swap(ZValList_AppleEvent& iOther);

	ZValList_AppleEvent();
	ZValList_AppleEvent(const ZValList_AppleEvent& iOther);
	~ZValList_AppleEvent();
	ZValList_AppleEvent& operator=(const ZValList_AppleEvent& iOther);

	ZValList_AppleEvent(const AEDescList& iOther);
	ZValList_AppleEvent& operator=(const AEDescList& iOther);

// ZValList protocol
	size_t Count() const;

	void Clear();

	bool QGet(size_t iIndex, ZVal_AppleEvent& oVal) const;
	ZVal_AppleEvent DGet(size_t iIndex, const ZVal_AppleEvent& iDefault) const;
	ZVal_AppleEvent Get(size_t iIndex) const;

	void Set(size_t iIndex, const AEDesc& iVal);

	void Erase(size_t iIndex);

	void Insert(size_t iIndex, const AEDesc& iVal);

	void Append(const AEDesc& iVal);
	void Append(const ZVal_AppleEvent& iVal);
	};

// =================================================================================================
#pragma mark -
#pragma mark * ZValMap_AppleEvent

class ZValMap_AppleEvent
:	public AERecord
	{
	ZOOLIB_DEFINE_OPERATOR_BOOL_TYPES(ZValMap_AppleEvent,
		operator_bool_generator_type, operator_bool_type);

public:
	typedef ZValMapIndex_T<ZValMap_AppleEvent> Index_t;

	operator operator_bool_type() const;

	void swap(ZValMap_AppleEvent& iOther);

	ZValMap_AppleEvent();
	ZValMap_AppleEvent(const ZValMap_AppleEvent& iOther);
	~ZValMap_AppleEvent();
	ZValMap_AppleEvent& operator=(const ZValMap_AppleEvent& iOther);

	ZValMap_AppleEvent(const AERecord& iOther);
	ZValMap_AppleEvent& operator=(const AERecord& iOther);

// ZValMap protocol
	void Clear();

	bool QGet(AEKeyword iName, ZVal_AppleEvent& oVal) const;
	bool QGet(const std::string& iName, ZVal_AppleEvent& oVal) const;
	bool QGet(Index_t iPropIter, ZVal_AppleEvent& oVal) const;

	ZVal_AppleEvent DGet(AEKeyword iName, const ZVal_AppleEvent& iDefault) const;
	ZVal_AppleEvent DGet(const std::string& iName, const ZVal_AppleEvent& iDefault) const;
	ZVal_AppleEvent DGet(Index_t iIndex, const ZVal_AppleEvent& iDefault) const;

	ZVal_AppleEvent Get(AEKeyword iName) const;
	ZVal_AppleEvent Get(const std::string& iName) const;
	ZVal_AppleEvent Get(Index_t iIndex) const;

	void Set(AEKeyword iName, const AEDesc& iVal);
	void Set(const std::string& iName, const AEDesc& iVal);
	void Set(Index_t iIndex, const AEDesc& iVal);

	void Erase(AEKeyword iName);
	void Erase(const std::string& iName);
	void Erase(Index_t iIndex);

// Our protocol
	AERecord& OParam();

	bool QGetAttr(AEKeyword iName, ZVal_AppleEvent& oVal) const;
	ZVal_AppleEvent DGetAttr(AEKeyword iName, const ZVal_AppleEvent& iDefault) const;
	ZVal_AppleEvent GetAttr(AEKeyword iName) const;
	void SetAttr(AEKeyword iName, const AEDesc& iVal);

	Index_t Begin() const;
	Index_t End() const;

	AEKeyword KeyOf(Index_t iIndex) const;
	std::string NameOf(Index_t iIndex) const;

	Index_t IndexOf(AEKeyword iName) const;
	Index_t IndexOf(const std::string& iName) const;

	Index_t IndexOf(const ZValMap_AppleEvent& iOther, const Index_t& iOtherIndex) const;

private:
	size_t pCount() const;
	AEKeyword pKeyOf(size_t iIndex) const;
	};

NAMESPACE_ZOOLIB_END

// =================================================================================================
#pragma mark -
#pragma mark * std::swap

namespace std {

inline void swap(ZOOLIB_PREFIX::ZVal_AppleEvent& a, ZOOLIB_PREFIX::ZVal_AppleEvent& b)
	{ a.swap(b); }

inline void swap(ZOOLIB_PREFIX::ZValList_AppleEvent& a, ZOOLIB_PREFIX::ZValList_AppleEvent& b)
	{ a.swap(b); }

inline void swap(ZOOLIB_PREFIX::ZValMap_AppleEvent& a, ZOOLIB_PREFIX::ZValMap_AppleEvent& b)
	{ a.swap(b); }

} // namespace std

#endif // ZCONFIG_SPI_Enabled(AppleEvent)

#endif // __ZVal_AppleEvent__
