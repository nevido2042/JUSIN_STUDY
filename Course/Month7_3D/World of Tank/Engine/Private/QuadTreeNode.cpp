#include "QuadTreeNode.h"

CQuadTreeNode::CQuadTreeNode()
{
}

HRESULT CQuadTreeNode::Initialize()
{
	return S_OK;
}

CQuadTreeNode* CQuadTreeNode::Create()
{
	CQuadTreeNode* pInstance = new CQuadTreeNode();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CQuadTreeNode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuadTreeNode::Free()
{
	for (int i = 0; i < 4; ++i)
		if (m_pChildren[i]) Safe_Release(m_pChildren[i]);
}
