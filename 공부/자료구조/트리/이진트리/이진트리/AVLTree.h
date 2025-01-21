#pragma once
#include "BinarySearchTree.h"

template<typename T>
class CAVLTree final:
    public CBinarySearchTree<T>
{
public:
	//virtual void Insert(T _Data) override;
	CNode<T>* Insert(T _Data, CNode<T>** _ppRoot = nullptr);
	virtual CNode<T>* Remove(T _Target) override;
private:
	//void Insert(CNode<T>** _ppNode, T _Data);
	int Get_Height(CNode<T>* _pNode);
	int Get_Height_Diff(CNode<T>* _pNode);
	CNode<T>* Rotate_LL(CNode<T>* _pNode);
	CNode<T>* Rotate_RR(CNode<T>* _pNode);
	CNode<T>* Rotate_LR(CNode<T>* _pNode);
	CNode<T>* Rotate_RL(CNode<T>* _pNode);
	CNode<T>* Rebalance();
	CNode<T>* Rebalance(CNode<T>* _pNode);
};

//template<typename T>
//inline void CAVLTree<T>::Insert(T _Data)
//{
//	/*CBinarySearchTree<T>::Insert(_Data);
//	Rebalance();*/
//
//	if (this->m_pRootNode == nullptr)
//	{
//		this->m_pRootNode = new CNode<T>;
//		this->m_pRootNode->Set_Data(_Data);
//	}
//	else if (_Data < this->m_pRootNode->Get_Data())
//	{
//		CNode<T>* pLeftNode = this->m_pRootNode->Get_Left();
//		Insert(&pLeftNode, _Data);
//		this->m_pRootNode = Rebalance();
//	}
//	else if (_Data > this->m_pRootNode->Get_Data())
//	{
//		CNode<T>* pRightNode = this->m_pRootNode->Get_Right();
//		Insert(&pRightNode, _Data);
//		this->m_pRootNode =Rebalance();
//	}
//	else
//	{
//		return;    // 키의 중복을 허용하지 않는다. 
//	}
//
//	return;
//}
//
//template<typename T>
//inline void CAVLTree<T>::Insert(CNode<T>** _ppNode, T _Data)
//{
//	if (*_ppNode == nullptr)
//	{
//		*_ppNode = new CNode<T>;
//		(*_ppNode)->Set_Data(_Data);
//	}
//	else if (_Data < (*_ppNode)->Get_Data())
//	{
//		CNode<T>* pLeftNode = (*_ppNode)->Get_Left();
//		Insert(&pLeftNode, _Data);
//		this->m_pRootNode = Rebalance();
//	}
//	else if (_Data > (*_ppNode)->Get_Data())
//	{
//		CNode<T>* pRightNode = (*_ppNode)->Get_Right();
//		Insert(&pRightNode, _Data);
//		this->m_pRootNode = Rebalance();
//	}
//	else
//	{
//		return;    // 키의 중복을 허용하지 않는다. 
//	}
//
//	return;
//}

template<typename T>
inline CNode<T>* CAVLTree<T>::Insert(T _Data, CNode<T>** _ppRoot)
{
	if (_ppRoot == nullptr)
	{
		_ppRoot = &(this->m_pRootNode);
	}

	if (*_ppRoot == nullptr)
	{
		*_ppRoot = new CNode<T>;
		(*_ppRoot)->Set_Data(_Data);
	}
	else if (_Data < (*_ppRoot)->Get_Data())
	{
		CNode<T>* pLeftNode = (*_ppRoot)->Get_Left();
		(*_ppRoot)->Set_Left(Insert(_Data, &pLeftNode));
		*_ppRoot = Rebalance(*_ppRoot);
	}
	else if (_Data > (*_ppRoot)->Get_Data())
	{
		CNode<T>* pRightNode = (*_ppRoot)->Get_Right();
		(*_ppRoot)->Set_Right(Insert(_Data, &pRightNode));
		*_ppRoot = Rebalance(*_ppRoot);
	}
	else
	{
		return nullptr;    // 키의 중복을 허용하지 않는다. 
	}

	return *_ppRoot;
}

template<typename T>
inline CNode<T>* CAVLTree<T>::Remove(T _Target)
{
	CNode<T>* pDeleteNode = CBinarySearchTree<T>::Remove(_Target);
	Rebalance();
	return pDeleteNode;
}

template<typename T>
inline int CAVLTree<T>::Get_Height(CNode<T>* _pNode)
{
	int iLeft_Height(0);
	int iRight_Height(0);

	if (_pNode == nullptr)
	{
		return 0;
	}

	iLeft_Height = Get_Height(_pNode->Get_Left());
	iRight_Height = Get_Height(_pNode->Get_Right());

	if (iLeft_Height > iRight_Height)
	{
		return iLeft_Height + 1;
	}
	else
	{
		return iRight_Height + 1;
	}

}

template<typename T>
inline int CAVLTree<T>::Get_Height_Diff(CNode<T>* _pNode)
{
	int iLeft_Sub_Tree_Height(0);
	int iRight_Sub_Tree_Height(0);

	if (_pNode == nullptr)
	{
		return 0;
	}

	iLeft_Sub_Tree_Height = Get_Height(_pNode->Get_Left());
	iRight_Sub_Tree_Height = Get_Height(_pNode->Get_Right());
	
	return iLeft_Sub_Tree_Height - iRight_Sub_Tree_Height;
}

template<typename T>
inline CNode<T>* CAVLTree<T>::Rotate_LL(CNode<T>* _pNode)
{
	CNode<T>* pParentNode(nullptr);
	CNode<T>* pChildNode(nullptr);

	pParentNode = _pNode;
	pChildNode = _pNode->Get_Left();

	pParentNode->Change_Left_Sub_Tree(pChildNode->Get_Right());
	pChildNode->Change_Right_Sub_Tree(pParentNode);

	return pChildNode;
}

template<typename T>
inline CNode<T>* CAVLTree<T>::Rotate_RR(CNode<T>* _pNode)
{
	CNode<T>* pParentNode(nullptr);
	CNode<T>* pChildNode(nullptr);

	pParentNode = _pNode;
	pChildNode = _pNode->Get_Right();

	pParentNode->Change_Right_Sub_Tree(pChildNode->Get_Left());
	pChildNode->Change_Left_Sub_Tree(pParentNode);

	return pChildNode;
}

template<typename T>
inline CNode<T>* CAVLTree<T>::Rotate_LR(CNode<T>* _pNode)
{
	CNode<T>* pParentNode(nullptr);
	CNode<T>* pChildNode(nullptr);

	pParentNode = _pNode;
	pChildNode = _pNode->Get_Left();

	pParentNode->Change_Left_Sub_Tree(Rotate_RR(pChildNode));

	return pParentNode;
}

template<typename T>
inline CNode<T>* CAVLTree<T>::Rotate_RL(CNode<T>* _pNode)
{
	CNode<T>* pParentNode(nullptr);
	CNode<T>* pChildNode(nullptr);

	pParentNode = _pNode;
	pChildNode = _pNode->Get_Right();

	pParentNode->Change_Right_Sub_Tree(Rotate_LL(pChildNode));

	return pParentNode;
}

template<typename T>
inline CNode<T>* CAVLTree<T>::Rebalance()
{
	int iHeight_Diff = Get_Height_Diff(this->m_pRootNode);

	if (iHeight_Diff > 1)
	{
		if (Get_Height_Diff(this->m_pRootNode->Get_Left()) > 0)
		{
			this->m_pRootNode = Rotate_LL(this->m_pRootNode);
		}
		else
		{
			this->m_pRootNode = Rotate_LR(this->m_pRootNode);
		}
	}

	if (iHeight_Diff < -1)
	{
		if (Get_Height_Diff(this->m_pRootNode->Get_Right()) < 0)
		{
			this->m_pRootNode = Rotate_RR(this->m_pRootNode);
		}
		else
		{
			this->m_pRootNode = Rotate_RL(this->m_pRootNode);
		}
	}

	return this->m_pRootNode;
}

template<typename T>
inline CNode<T>* CAVLTree<T>::Rebalance(CNode<T>* _pRoot)
{
	int iHeight_Diff = Get_Height_Diff(_pRoot);

	if (iHeight_Diff > 1)
	{
		if (Get_Height_Diff(_pRoot->Get_Left()) > 0)
		{
			_pRoot = Rotate_LL(_pRoot);
		}
		else
		{
			_pRoot = Rotate_LR(_pRoot);
		}
	}

	if (iHeight_Diff < -1)
	{
		if (Get_Height_Diff(_pRoot->Get_Right()) < 0)
		{
			_pRoot = Rotate_RR(_pRoot);
		}
		else
		{
			_pRoot = Rotate_RL(_pRoot);
		}
	}

	return _pRoot;
}
