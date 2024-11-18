#pragma once
#include "Define.h"
template<typename T>
class CNode
{
	T m_Data;
	CNode* m_pLeft;
	CNode* m_pRight;
public:
	CNode();

	CNode* Get_Left() const
	{
		return m_pLeft;
	}
	void Set_Left(CNode* _pNode)
	{
		m_pLeft = _pNode;
	}
	CNode* Get_Right()
	{
		return m_pRight;
	}
	void Set_Right(CNode* _pNode)
	{
		m_pRight = _pNode;
	}
	T& Get_Data()
	{
		return m_Data;
	}
	void Set_Data(T _Data)
	{
		m_Data = _Data;
	}
public:
	void Make_Left_Sub_Tree(CNode* _pMain, CNode* _pSub);
	void Make_Right_Sub_Tree(CNode* _pMain, CNode* _pSub);
};

template<typename T>
inline CNode<T>::CNode()
	:m_pLeft(nullptr), m_pRight(nullptr)
{
	memset(&m_Data, 0, sizeof(T));
}

template<typename T>
inline void CNode<T>::Make_Left_Sub_Tree(CNode* _pMain, CNode* _pSub)
{
	Safe_Delete(_pMain->Get_Left());

	_pMain->Set_Left(_pSub);
}

template<typename T>
inline void CNode<T>::Make_Right_Sub_Tree(CNode* _pMain, CNode* _pSub)
{
	Safe_Delete(_pMain->Get_Right());

	_pMain->Set_Right(_pSub);
}
