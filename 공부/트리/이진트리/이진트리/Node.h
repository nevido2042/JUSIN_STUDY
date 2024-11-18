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
	void Inorder_Traverse(void (*_pAction)(CNode*));
	void Posorder_Traverse(void (*_pAction)(CNode*));
	void Delete_Tree();
	static void Print_Data(CNode* _pNode);
	static void Delete_Tree(CNode* _pNode);
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

template<typename T>
inline void CNode<T>::Inorder_Traverse(void (*_pAction)(CNode*))
{
	if (Get_Left())
	{
		Get_Left()->Inorder_Traverse(_pAction);
	}

	_pAction(this);

	if (Get_Right())
	{
		Get_Right()->Inorder_Traverse(_pAction);
	}
}

template<typename T>
inline void CNode<T>::Posorder_Traverse(void(*_pAction)(CNode*))
{
	if (Get_Left())
	{
		Get_Left()->Posorder_Traverse(_pAction);
	}

	if (Get_Right())
	{
		Get_Right()->Posorder_Traverse(_pAction);
	}

	_pAction(this);
}

template<typename T>
inline void CNode<T>::Delete_Tree()
{
	Posorder_Traverse(&CNode<int>::Delete_Tree);
}

template<typename T>
inline void CNode<T>::Print_Data(CNode* _pNode)
{
	cout << _pNode->Get_Data() << endl;
}

template<typename T>
inline void CNode<T>::Delete_Tree(CNode* _pNode)
{
	Safe_Delete(_pNode);
}
