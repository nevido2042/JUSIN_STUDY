#pragma once
#include "pch.h"
template<typename T1, typename T2>
class CMap
{
	class CNode
	{
		pair<T1, T2> m_Pair;
		//T1 m_Key;
		//T2 m_Value;
		
		CNode* m_pLeft;
		CNode* m_pRight;
	public:
		CNode();
		~CNode();
	public:
		void Initialize();
		void Release();
	public:
		/*void Set_Key(T1 _Key)
		{
			m_Key = m_Key;
		}
		void Set_Value(T2 _Value)
		{
			m_Value = _Value;
		}*/
		void Set_Pair(pair<T1, T2> _Pair)
		{
			m_Pair = _Pair;
		}
		pair<T1, T2> Get_Pair()
		{
			return m_Pair;
		}
	};

	class CIterator
	{
		CNode* m_pNode;
	public:
		CIterator();
		~CIterator();
	public:
		void Set_Node(CNode* _pNode)
		{
			m_pNode = _pNode;
		}
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
	CIterator& Find(const T1 _Key);

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
	//NewNode->Set_Key(_Pair.first);
	//NewNode->Set_Value(_Pair.second);
	NewNode->Set_Pair(_Pair);

	m_Root = NewNode;
}

template<typename T1, typename T2>
inline CMap<T1, T2>::CIterator& CMap<T1, T2>::Find(const T1 _Key)
{
	//생성한 이터레이터들을 기억했다가. 삭제하는 기능이 필요 할 것 같다.
	CIterator* pIter = new CIterator;

	if (_Key == m_Root->Get_Pair().first)
	{
		pIter->Set_Node(m_Root);		
	}
	return *pIter;

}

template<typename T1, typename T2>
inline CMap<T1, T2>::CNode::CNode()
	:m_pLeft(nullptr), m_pRight(nullptr)
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

template<typename T1, typename T2>
inline CMap<T1, T2>::CIterator::CIterator()
	:m_pNode(nullptr)
{
}

template<typename T1, typename T2>
inline CMap<T1, T2>::CIterator::~CIterator()
{
}
