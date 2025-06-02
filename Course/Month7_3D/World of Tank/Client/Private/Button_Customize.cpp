#include "Button_Customize.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "Layer.h"

CButton_Customize::CButton_Customize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CButton_Customize::CButton_Customize(const CButton_Customize& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CButton_Customize::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton_Customize::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CButton_Customize::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CButton_Customize::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::LBUTTON)) && isPick(g_hWnd))
	{
		CGameManager* pGameManager = GET_GAMEMANAGER;
		pGameManager->PlaySound_Button();

		for (CGameObject* pGameObject : m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::HANGER), TEXT("Layer_Button"))->Get_GameObjects())
		{
			pGameObject->Set_Active(false);
		}

		for (CGameObject* pGameObject : m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::HANGER), TEXT("Layer_Button_Customize"))->Get_GameObjects())
		{
			pGameObject->Set_Active(true);
		}
	}
}

void CButton_Customize::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CButton_Customize::Render()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	_float fAlpha = { 0.3f };
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

	m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), TEXT("전차 꾸미기"), _float2(m_fX - m_fSizeX * 0.35f, m_fY - m_fSizeY * 0.3f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.5f * UI_RATIO);

	return S_OK;
}

HRESULT CButton_Customize::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Button_Login"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CButton_Customize* CButton_Customize::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CButton_Customize* pInstance = new CButton_Customize(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CButton_Customize");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton_Customize::Clone(void* pArg)
{
	CButton_Customize* pInstance = new CButton_Customize(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CButton_Customize");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton_Customize::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
