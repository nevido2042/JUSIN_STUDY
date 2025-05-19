#include "Button_Login.h"

#include "GameInstance.h"
#include "Level_Loading.h"

CButton_Login::CButton_Login(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CButton_Login::CButton_Login(const CButton_Login& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CButton_Login::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton_Login::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CButton_Login::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CButton_Login::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::LBUTTON)) && isPick(g_hWnd))
	{
		//현재 레벨아 레벨 체인지 될거야~
		m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::HANGER));
	}
	//else if (m_pGameInstance->Key_Pressing(DIK_R))
	//	Resize(g_iWinSizeX, g_iWinSizeY);
}

void CButton_Login::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CButton_Login::Render()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
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

HRESULT CButton_Login::Ready_Components()
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

CButton_Login* CButton_Login::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CButton_Login* pInstance = new CButton_Login(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CButton_Login");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton_Login::Clone(void* pArg)
{
	CButton_Login* pInstance = new CButton_Login(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CButton_Login");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton_Login::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
