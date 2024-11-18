#pragma once
#include "Node.h"

template<typename T>
class CBinarySearchTree
{
	CNode<T>* m_pRootNode;
public:
	CBinarySearchTree();
	~CBinarySearchTree();
private:
	void Release();
public:
	void Insert(T _Data);
	CNode<T>* Remove(T _Target);
	CNode<T>* Search(T _Target);
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
inline CBinarySearchTree<T>::~CBinarySearchTree()
{
	Release();
}

template<typename T>
inline void CBinarySearchTree<T>::Release()
{
	Postorder_Traverse(CNode<int>::Delete_Tree);
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
inline CNode<T>* CBinarySearchTree<T>::Remove(T _Target)
{
	CNode<T>* pVirtualRootNode = new CNode<T>;
	CNode<T>* pParentNode = pVirtualRootNode;
	CNode<T>* pCurrentNode = m_pRootNode;
	CNode<T>* pDeleteNode(nullptr);

	pVirtualRootNode->Set_Right(m_pRootNode);

	while (pCurrentNode != nullptr && pCurrentNode->Get_Data() != _Target)
	{
		pParentNode = pCurrentNode;

		if (_Target < pCurrentNode->Get_Data())
		{
			pCurrentNode = pCurrentNode->Get_Left();
		}
		else
		{
			pCurrentNode = pCurrentNode->Get_Right();
		}
	}

	if (pCurrentNode == nullptr) //삭제 대상이 존재하지 않는다면
	{
		return nullptr;
	}

	pDeleteNode = pCurrentNode;

	//첫 번째 경우: 삭제 대상이 단말 노드인 경우
	if (pDeleteNode->Get_Left() == nullptr && pDeleteNode->Get_Right() == nullptr)
	{
		if (pParentNode->Get_Left() == pDeleteNode)
		{
			pParentNode->Remove_Left_Sub_Tree();
		}
		else
		{
			pParentNode->Remove_Right_Sub_Tree();
		}
	}
	//두 번째 경우: 삭제 대상이 하나의 자식 노드를 갖는 경우
	else if (pDeleteNode->Get_Left() == nullptr || pDeleteNode->Get_Right() == nullptr)
	{
		CNode<T>* pDeleteNode_ChildNode(nullptr);

		if (pDeleteNode->Get_Left() != nullptr)
		{
			pDeleteNode_ChildNode = pDeleteNode->Get_Left();
		}
		else
		{
			pDeleteNode_ChildNode = pDeleteNode->Get_Right();
		}

		if (pParentNode->Get_Left() == pDeleteNode)
		{
			pParentNode->Change_Left_Sub_Tree(pDeleteNode_ChildNode);
		}
		else
		{
			pParentNode->Change_Right_Sub_Tree(pDeleteNode_ChildNode);
		}
	}
	//세 번째 경우: 두 개의 자식 노드를 모두 갖는 경우
	else
	{
		CNode<T>* pReplaceNode = pDeleteNode->Get_Right(); //대체 노드를 가리킴 //왜 오른쪽?
		CNode<T>* pReplaceNode_Parent = pDeleteNode;
		int DeleteData(0);

		while (pReplaceNode->Get_Left() != nullptr)
		{
			pReplaceNode_Parent = pReplaceNode;
			pReplaceNode = pReplaceNode->Get_Left();
		}

		DeleteData = pDeleteNode->Get_Data();
		pDeleteNode->Set_Data(pReplaceNode->Get_Data());

		if (pReplaceNode_Parent->Get_Left() == pReplaceNode)
		{
			pReplaceNode_Parent->Change_Left_Sub_Tree(pReplaceNode->Get_Right());
		}
		else
		{
			pReplaceNode_Parent->Change_Right_Sub_Tree(pReplaceNode->Get_Right());
		}

		pDeleteNode = pReplaceNode;
		pDeleteNode->Set_Data(DeleteData);
	}

	if (pVirtualRootNode->Get_Right() != m_pRootNode)
	{
		m_pRootNode = pVirtualRootNode->Get_Right();
	}

	Safe_Delete(pVirtualRootNode);
	return pDeleteNode;
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
