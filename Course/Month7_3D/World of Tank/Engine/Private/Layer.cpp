#include "Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

CComponent* CLayer::Get_Component(const _wstring& strComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	
	return (*iter)->Get_Component(strComponentTag);
}

CGameObject* CLayer::Get_GameObject(_uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	return (*iter);
}

CGameObject* CLayer::Find_GameObject_By_Name(const _wstring& strName)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (pGameObject && strName == pGameObject->GetName()) // 문자열 비교
		{
			return pGameObject;
		}
	}
	return nullptr; // 못 찾으면 nullptr 반환
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); )
	{
		if (true == (*iter)->Get_IsDestroyed())
		{
			Safe_Release(*iter);
			iter = m_GameObjects.erase(iter);
			continue;
		}

		if (nullptr != *iter && (*iter)->Get_isActive())
		{
			(*iter)->Priority_Update(fTimeDelta);
			++iter;
		}
		else
		{
			++iter;
		}

	}

	//for (auto& pGameObject : m_GameObjects)
	//{
	//	if (nullptr != pGameObject)
	//		pGameObject->Priority_Update(fTimeDelta);
	//}
		
}

void CLayer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject && pGameObject->Get_isActive())
			pGameObject->Update(fTimeDelta);

	}
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject && pGameObject->Get_isActive())
			pGameObject->Late_Update(fTimeDelta);

	}
}

CLayer* CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);
	m_GameObjects.clear();
}
