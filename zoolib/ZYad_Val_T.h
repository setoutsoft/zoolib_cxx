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

#ifndef __ZYad_Val_T_h__
#define __ZYad_Val_T_h__ 1
#include "zconfig.h"

#include "zoolib/ZYad.h"
#include "zoolib/ZStream_Data_T.h"

namespace ZooLib {

// =================================================================================================
// MARK: - ZYadR_Val_T

template <class Val_t>
class ZYadR_Val_T
:	public virtual ZYadR
	{
public:
	typedef ZYadR_Val_T YadBase_t;

	ZYadR_Val_T()
		{}

	template <class P0>
	ZYadR_Val_T(const P0& i0)
	:	fVal(i0)
		{}

	template <class P0, class P1>
	ZYadR_Val_T(const P0& i0, const P1& i1)
	:	fVal(i0, i1)
		{}

// Our protocol
	Val_t GetVal()
		{ return fVal; }

protected:
	const Val_t fVal;
	};

// =================================================================================================
// MARK: - ZYadStreamRPos_Val_T

template <class Data_t>
class ZYadStreamRPos_Val_T
:	public ZYadStreamR
,	public ZStreamerRPos_T<ZStreamRPos_Data_T<Data_t> >
	{
public:
	ZYadStreamRPos_Val_T(const Data_t& iData)
	:	ZStreamerRPos_T<ZStreamRPos_Data_T<Data_t> >(iData)
		{}

// From ZYadR
	bool IsSimple(const ZYadOptions& iOptions)
		{ return this->GetStreamRPos().GetSize() <= iOptions.fRawChunkSize; }
	};

// =================================================================================================
// MARK: - ZYadSatRPos_Val_T

template <class Seq_t>
class ZYadSatRPos_Val_T
:	public ZYadSatRPos
	{
public:
	ZYadSatRPos_Val_T(const Seq_t& iSeq)
	:	fSeq(iSeq)
	,	fPosition(0)
		{}

	ZYadSatRPos_Val_T(const Seq_t& iSeq, uint64 iPosition)
	:	fSeq(iSeq)
	,	fPosition(iPosition)
		{}

// From ZYadSeqR via ZYadSatRPos
	virtual ZRef<ZYadR> ReadInc()
		{
		if (fPosition < fSeq.Count())
			return sYadR(fSeq.Get(fPosition++));
		return null;
		}

// From ZYadSeqRClone via ZYadSatRPos
	virtual ZRef<ZYadSeqRClone> Clone()
		{ return new ZYadSatRPos_Val_T(fSeq, fPosition); }

// From ZYadSeqRPos
	virtual uint64 GetPosition()
		{ return fPosition; }

	virtual void SetPosition(uint64 iPosition)
		{ fPosition = iPosition; }

	virtual uint64 GetSize()
		{ return fSeq.Count(); }

// From ZYadSat
	virtual uint64 Count()
		{ return fSeq.Count(); }

	virtual ZRef<ZYadR> ReadAt(uint64 iPosition)
		{
		if (iPosition < fSeq.Count())
			return sYadR(fSeq.Get(iPosition));
		return null;
		}

// Our protocol
	const Seq_t& GetSeq()
		{ return fSeq; }

protected:
	const Seq_t fSeq;
	uint64 fPosition;
	};

// =================================================================================================
// MARK: - ZYadSatRPos_Val_Self_T

// Urgh, quite an ugly name.

template <class Self_t, class Seq_t>
class ZYadSatRPos_Val_Self_T
:	public ZYadSatRPos
	{
public:
	typedef ZYadSatRPos_Val_Self_T YadSeqBase_t;

protected:
	ZYadSatRPos_Val_Self_T(const Seq_t& iSeq)
	:	fSeq(iSeq)
	,	fPosition(0)
		{}

	ZYadSatRPos_Val_Self_T(const Seq_t& iSeq, uint64 iPosition)
	:	fSeq(iSeq)
	,	fPosition(iPosition)
		{}

public:
// From ZYadSeqR via ZYadSatRPos
	virtual ZRef<ZYadR> ReadInc()
		{
		if (fPosition < fSeq.Count())
			return sYadR(fSeq.Get(fPosition++));
		return null;
		}

// From ZYadSeqRClone via ZYadSatRPos
	virtual ZRef<ZYadSeqRClone> Clone()
		{ return new Self_t(fSeq, fPosition); }

// From ZYadSeqRPos
	virtual uint64 GetPosition()
		{ return fPosition; }

	virtual void SetPosition(uint64 iPosition)
		{ fPosition = iPosition; }

	virtual uint64 GetSize()
		{ return fSeq.Count(); }

// From ZYadSat
	virtual uint64 Count()
		{ return fSeq.Count(); }

	virtual ZRef<ZYadR> ReadAt(uint64 iPosition)
		{
		if (iPosition < fSeq.Count())
			return sYadR(fSeq.Get(iPosition));
		return null;
		}

protected:
	const Seq_t fSeq;
	uint64 fPosition;
	};

// =================================================================================================
// MARK: - ZYadMatRPos_Val_T

template <class Map_t, class Index_P = typename Map_t::Index_t>
class ZYadMatRPos_Val_T
:	public ZYadMatRPos
	{
public:
	typedef Index_P Index_t;

	ZYadMatRPos_Val_T(const Map_t& iMap)
	:	fMap(iMap)
	,	fIndex(fMap.Begin())
		{}

	ZYadMatRPos_Val_T(const Map_t& iMap, const Index_t& iIndex)
	:	fMap(iMap)
	,	fIndex(fMap.IndexOf(iMap, iIndex))
		{}

// From ZYadMapR via ZYadMatRPos
	ZRef<ZYadR> ReadInc(std::string& oName)
		{
		if (fIndex != fMap.End())
			{
			oName = fMap.NameOf(fIndex);
			return sYadR(fMap.Get(fIndex++));
			}
		return null;
		}

// From ZYadMapRClone via ZYadMatRPos
	virtual ZRef<ZYadMapRClone> Clone()
		{ return new ZYadMatRPos_Val_T(fMap, fIndex); }

// From ZYadMatRPos
	void SetPosition(const std::string& iName)
		{ fIndex = fMap.IndexOf(iName); }

// From ZYadMat
	virtual ZRef<ZYadR> ReadAt(const std::string& iName)
		{
		Index_t theIndex = fMap.IndexOf(iName);
		if (theIndex != fMap.End())
			return sYadR(fMap.Get(theIndex));
		return null;
		}
		
// Our protocol
	const Map_t& GetMap()
		{ return fMap; }

protected:
	const Map_t fMap;
	Index_t fIndex;
	};

// =================================================================================================
// MARK: - ZYadMatRPos_Val_Self_T

template <class Self_t, class Map_t, class Index_P = typename Map_t::Index_t>
class ZYadMatRPos_Val_Self_T
:	public ZYadMatRPos
	{
public:
	typedef Index_P Index_t;

protected:
	typedef ZYadMatRPos_Val_Self_T YadMapBase_t;
	ZYadMatRPos_Val_Self_T(const Map_t& iMap)
	:	fMap(iMap)
	,	fIndex(fMap.Begin())
		{}

	ZYadMatRPos_Val_Self_T(const Map_t& iMap, const Index_t& iIndex)
	:	fMap(iMap)
	,	fIndex(iIndex)
		{}

public:
// From ZYadMapR via ZYadMatRPos
	ZRef<ZYadR> ReadInc(std::string& oName)
		{
		if (fIndex != fMap.End())
			{
			oName = fMap.NameOf(fIndex);
			return sYadR(fMap.Get(fIndex++));
			}
		return null;
		}

// From ZYadMapRClone via ZYadMatRPos
	virtual ZRef<ZYadMapRClone> Clone()
		{ return new Self_t(fMap, fIndex); }

// From ZYadMatRPos
	void SetPosition(const std::string& iName)
		{ fIndex = fMap.IndexOf(iName); }

// From ZYadMat
	virtual ZRef<ZYadR> ReadAt(const std::string& iName)
		{
		Index_t theIndex = fMap.IndexOf(iName);
		if (theIndex != fMap.end())
			return fMap.Get(theIndex);
		return null;
		}

// Our protocol
	const Map_t& GetMap()
		{ return fMap; }

protected:
	const Map_t fMap;
	Index_t fIndex;
	};

} // namespace ZooLib

#endif // __ZYad_Val_T_h__
