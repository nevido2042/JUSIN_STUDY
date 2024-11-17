#pragma once
#include "pch.h"
template<typename T1, typename T2>
class CMap
{
	class CNode
	{
		//pair<T1, T2> m_Pair;
		T1 m_Key;
		T2 m_Value;
		
		CNode* m_Left;
		CNode* m_Right;
	public:
		CNode();
		~CNode();
	public:
		void Initialize();
		void Release();
	public:
		void Set_Key(T1 _Key)
		{
			m_Key = m_Key;
		}
		void Set_Value(T2 _Value)
		{
			m_Value = _Value;
		}
	};

	class CIterator
	{
		
	};

	CNode* m_Root;
public:
	CMap();
	~CMap();
public:
	void Initialize();
	void Release();
public:
	void Insert(pair<T1, T2> _Pair);

};

template<typename T1, typename T2>
inline CMap<T1, T2>::CMap()
	:m_Root(nullptr)
{
}

template<typename T1, typename T2>
inline CMap<T1, T2>::~CMap()
{
}

template<typename T1, typename T2>
inline void CMap<T1, T2>::Initialize()
{
}

template<typename T1, typename T2>
inline void CMap<T1, T2>::Release()
{
}

template<typename T1, typename T2>
inline void CMap<T1, T2>::Insert(pair<T1, T2> _Pair)
{
	CNode* NewNode = new CNode;
	NewNode->Set_Key(_Pair.first);
	NewNode->Set_Value(_Pair.second);

	m_Root = NewNode;
}

template<typename T1, typename T2>
inline CMap<T1, T2>::CNode::CNode()
	:m_Left(nullptr), m_Right(nullptr)
{
}

template<typename T1, typename T2>
inline CMap<T1, T2>::CNode::~CNode()
{
}

template<typename T1, typename T2>
inline void CMap<T1, T2>::CNode::Initialize()
{
}

template<typename T1, typename T2>
inline void CMap<T1, T2>::CNode::Release()
{
}
