#include "Button_Fury.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "GameManager.h"

CButton_Fury::CButton_Fury(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CButton_Fury::CButton_Fury(const CButton_Fury& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CButton_Fury::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton_Fury::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CButton_Fury::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CButton_Fury::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::LBUTTON)) && isPick(g_hWnd))
	{
		if (m_bVisible == false)
			return;

		CGameManager* pGameManager = GET_GAMEMANAGER;
		pGameManager->PlaySound_Button();

		pGameManager->Set_Select_Tank(TANK::FURY);
		Active_This_Tank_Solo();
	}
}

void CButton_Fury::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CButton_Fury::Render()
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

	return S_OK;
}

HRESULT CButton_Fury::Active_This_Tank_Solo()
{
	m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Layer_Fury_Hanger"))->Set_Active(true);
	m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Layer_Tiger_Hanger"))->Set_Active(false);

	return S_OK;
}

HRESULT CButton_Fury::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_Button_Fury"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CButton_Fury* CButton_Fury::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CButton_Fury* pInstance = new CButton_Fury(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CButton_Fury");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton_Fury::Clone(void* pArg)
{
	CButton_Fury* pInstance = new CButton_Fury(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CButton_Fury");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton_Fury::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
