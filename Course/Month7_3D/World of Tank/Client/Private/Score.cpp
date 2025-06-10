#include "Score.h"

#include "GameInstance.h"

CScore::CScore(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CScore::CScore(const CScore& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CScore::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScore::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CScore::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CScore::Update(_float fTimeDelta)
{

}

void CScore::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CScore::Render()
{
	// 폰트 출력
	m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), TEXT("0 : 0"), _float2(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.01f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.4f * UI_RATIO);

	return S_OK;
}

HRESULT CScore::Ready_Components()
{


	return S_OK;
}

CScore* CScore::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CScore* pInstance = new CScore(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CScore");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CScore::Clone(void* pArg)
{
	CScore* pInstance = new CScore(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CScore");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScore::Free()
{
	__super::Free();

}
