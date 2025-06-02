#include "Background_UI.h"

#include "GameInstance.h"

CBackground_UI::CBackground_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CBackground_UI::CBackground_UI(const CBackground_UI& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CBackground_UI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackground_UI::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CBackground_UI::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CBackground_UI::Update(_float fTimeDelta)
{

}

void CBackground_UI::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CBackground_UI::Render()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float fAlpha = { 0.0f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackground_UI::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_Background_UI"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CBackground_UI* CBackground_UI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBackground_UI* pInstance = new CBackground_UI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBackground_UI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBackground_UI::Clone(void* pArg)
{
	CBackground_UI* pInstance = new CBackground_UI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBackground_UI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackground_UI::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
