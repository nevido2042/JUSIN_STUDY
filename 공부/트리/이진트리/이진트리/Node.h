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
	void Preorder_Traverse(void (*_pAction)(CNode*));
	void Inorder_Traverse(void (*_pAction)(CNode*));
	void Postorder_Traverse(void (*_pAction)(CNode*));
	void Delete_Tree();
	static void Print_Data(CNode* _pNode);
	static void Delete_Tree(CNode* _pNode);
	CNode* Remove_Left_Sub_Tree();
	CNode* Remove_Right_Sub_Tree();
	void Change_Left_Sub_Tree(CNode* _pNode);
	void Change_Right_Sub_Tree(CNode* _pNode);

};

template<typename T>
inline CNode<T>::CNode()
	:m_pLeft(nullptr), m_pRight(nullptr)
{
	memset(&m_Data, 0, sizeof(T));
}

template<typename T>
inline void CNode<T>::Preorder_Traverse(void(*_pAction)(CNode*))
{
	_pAction(this);

	if (Get_Left())
	{
		Get_Left()->Preorder_Traverse(_pAction);
	}

	if (Get_Right())
	{
		Get_Right()->Preorder_Traverse(_pAction);
	}
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
inline void CNode<T>::Postorder_Traverse(void(*_pAction)(CNode*))
{
	if (Get_Left())
	{
		Get_Left()->Postorder_Traverse(_pAction);
	}

	if (Get_Right())
	{
		Get_Right()->Postorder_Traverse(_pAction);
	}

	_pAction(this);
}

template<typename T>
inline void CNode<T>::Delete_Tree()
{
	Postorder_Traverse(&CNode<int>::Delete_Tree);
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

template<typename T>
inline CNode<T>* CNode<T>::Remove_Left_Sub_Tree()
{
	CNode* pRemoveNode = Get_Left();

	Set_Left(nullptr);

	return pRemoveNode;
}

template<typename T>
inline CNode<T>* CNode<T>::Remove_Right_Sub_Tree()
{
	CNode* pRemoveNode = Get_Right();

	Get_Right(nullptr);

	return pRemoveNode;
}

template<typename T>
inline void CNode<T>::Change_Left_Sub_Tree(CNode* _pNode)
{
	Set_Left(_pNode);
}

template<typename T>
inline void CNode<T>::Change_Right_Sub_Tree(CNode* _pNode)
{
	Set_Right(_pNode);
}
