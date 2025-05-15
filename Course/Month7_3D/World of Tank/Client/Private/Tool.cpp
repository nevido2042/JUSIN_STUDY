#include "Tool.h"
#include "GameInstance.h"

CTool::CTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject(pDevice, pContext)
{
}

CTool::CTool(const CTool& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CTool::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTool::Initialize(void* pArg)
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
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	ImGui::Render();  // ·»´õ¸µ Ã³¸®
}

HRESULT CTool::Render()
{

	return S_OK;
}

void CTool::Free()
{
	__super::Free();
}
