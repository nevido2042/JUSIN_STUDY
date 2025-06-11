#include "Collider_Manager.h"
#include "GameObject.h"
#include "Collider.h"

CCollider_Manager::CCollider_Manager()
{
}

HRESULT CCollider_Manager::Initialize(_uint iNumGroups)
{
	m_iNumGroups = iNumGroups;
	m_pGameObjects = new list<class CGameObject*>[m_iNumGroups];

	return S_OK;
}


HRESULT CCollider_Manager::Add_CollisionGroup(_uint iGroupIndex, CGameObject* pGameObject, wstring strComponentTag)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	static_cast<CCollider*>(pGameObject->Get_Component(strComponentTag))->Reset_Collision();

	m_pGameObjects[iGroupIndex].push_back(pGameObject);
	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CCollider_Manager::Out_CollisionGroup(_uint iGroupIndex, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	auto iter = m_pGameObjects[iGroupIndex].begin();
	while (iter != m_pGameObjects[iGroupIndex].end())
	{
		if (*iter == pGameObject)
		{
			Safe_Release(*iter);
			iter = m_pGameObjects[iGroupIndex].erase(iter);
		}
		else
			++iter;
	}

	return S_OK;
}

HRESULT CCollider_Manager::Clear_CollisionGroups()
{
	for (_uint i = 0; i < m_iNumGroups; ++i)
	{
		for (CGameObject* pGameObject : m_pGameObjects[i])
		{
			if (nullptr != pGameObject)
			{
				Safe_Release(pGameObject);
			}
		}
		m_pGameObjects[i].clear();
	}

	return S_OK;
}

#pragma message (" 여기 노말벡터 받는거 없애고 무조건 노말 계산하는걸로 바꿔도 괜찮을 지도")
void CCollider_Manager::Check_Collision(_uint iGroupIndex, CGameObject* pGameObject, wstring strComponentTag, wstring strOtherComponentTag/*, _vector* pOutNormal*/)
{	
	if (nullptr == pGameObject)
		return;

	CCollider* pCollider = static_cast<CCollider*>(pGameObject->Get_Component(strComponentTag));
	if(nullptr == pCollider)
		return;

	for (CGameObject* pOther : m_pGameObjects[iGroupIndex])
	{
		if (pOther == pGameObject)
			continue;

		CCollider* pOtherCollider = static_cast<CCollider*>(pOther->Get_Component(strOtherComponentTag));
		if(nullptr == pOtherCollider)
			continue;

		_vector vNormal = {};
		if (pCollider->Intersect(pOtherCollider, &vNormal))
			pGameObject->On_Collision_Stay(pOther, vNormal);

	}

	return;
}

CCollider_Manager* CCollider_Manager::Create(_uint iNumGroups)
{
	CCollider_Manager* pInstance = new CCollider_Manager();

	if (FAILED(pInstance->Initialize(iNumGroups)))
	{
		MSG_BOX("Failed to Created : CCollider_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumGroups; i++)
	{
		for (auto& GameObject : m_pGameObjects[i])
			Safe_Release(GameObject);

		m_pGameObjects[i].clear();
	}

	Safe_Delete_Array(m_pGameObjects);
}
