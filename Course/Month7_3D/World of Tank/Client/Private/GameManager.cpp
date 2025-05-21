#include "GameManager.h"

#include "GameInstance.h"

CGameManager::CGameManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CGameManager::CGameManager(const CGameManager& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CGameManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameManager::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};

	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("GameManager"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;


	return S_OK;
}

void CGameManager::Priority_Update(_float fTimeDelta)
{

}

void CGameManager::Update(_float fTimeDelta)
{
		
}

void CGameManager::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CGameManager::Render()
{

	return S_OK;
}

CGameManager* CGameManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGameManager* pInstance = new CGameManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGameManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGameManager::Clone(void* pArg)
{
	CGameManager* pInstance = new CGameManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGameManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameManager::Free()
{
	__super::Free();

}
