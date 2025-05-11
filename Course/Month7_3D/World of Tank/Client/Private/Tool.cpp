#include "Tool.h"
#include "GameInstance.h"

CTool::CTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CTool::Initialize()
{

	return S_OK;
}

void CTool::Priority_Update(_float fTimeDelta)
{

}

void CTool::Update(_float fTimeDelta)
{

}

void CTool::Late_Update(_float fTimeDelta)
{

}

HRESULT CTool::Render()
{

	return S_OK;
}

void CTool::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
