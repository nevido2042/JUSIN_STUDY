#pragma once
#include "Node.h"

template<typename T>
class CBinarySearchTree
{
	CNode<T>* m_pRootNode;
public:
	CBinarySearchTree();
public:
	void Insert(T _Data);
	CNode<T>* Search(T _Data);
	void Preorder_Traverse(void (*_pAction)(CNode<T>*));
	void Inorder_Traverse(void (*_pAction)(CNode<T>*));
	void Postorder_Traverse(void (*_pAction)(CNode<T>*));
};

template<typename T>
inline CBinarySearchTree<T>::CBinarySearchTree()
	:m_pRootNode(nullptr)
{

}

template<typename T>
inline void CBinarySearchTree<T>::Insert(T _Data)
{
	CNode<T>* pParentNode = nullptr;
	CNode<T>* pCurrentNode = m_pRootNode;
	CNode<T>* pNewNode = nullptr;

	while (pCurrentNode != nullptr)
	{
		if (_Data == pCurrentNode->Get_Data())
		{
			return;
		}

		pParentNode = pCurrentNode;

		if (pCurrentNode->Get_Data() > _Data)
		{
			pCurrentNode = pCurrentNode->Get_Left();
		}
		else
		{
			pCurrentNode = pCurrentNode->Get_Right();
		}
	}

	pNewNode = new CNode<T>;
	pNewNode->Set_Data(_Data);

	if (pParentNode != nullptr)
	{
		if (_Data < pParentNode->Get_Data())
		{
			pParentNode->Set_Left(pNewNode);
		}
		else
		{
			pParentNode->Set_Right(pNewNode);
		}
	}
	else
	{
		m_pRootNode = pNewNode;
	}
}

template<typename T>
inline CNode<T>* CBinarySearchTree<T>::Search(T _Target)
{
	CNode<T>* pCurrentNode = m_pRootNode;
	
	while (pCurrentNode != nullptr)
	{
		pCurrentNode->Get_Data();
		if (_Target == pCurrentNode->Get_Data())
			return pCurrentNode;
		else if (_Target < pCurrentNode->Get_Data())
		{
			pCurrentNode = pCurrentNode->Get_Left();
		}
		else
		{
			pCurrentNode = pCurrentNode->Get_Right();
		}
	}

	return nullptr;
}

template<typename T>
inline void CBinarySearchTree<T>::Preorder_Traverse(void(*_pAction)(CNode<T>*))
{
	m_pRootNode->Preorder_Traverse(_pAction);
}

template<typename T>
inline void CBinarySearchTree<T>::Inorder_Traverse(void(*_pAction)(CNode<T>*))
{
	m_pRootNode->Inorder_Traverse(_pAction);
}

template<typename T>
inline void CBinarySearchTree<T>::Postorder_Traverse(void(*_pAction)(CNode<T>*))
{
	m_pRootNode->Postorder_Traverse(_pAction);
}
