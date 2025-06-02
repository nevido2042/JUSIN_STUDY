#include "Button_Color.h"

#include "GameInstance.h"
#include "GameManager.h"

CButton_Color::CButton_Color(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CButton_Color::CButton_Color(const CButton_Color& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CButton_Color::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton_Color::Initialize(void* pArg)
{
	BUTTON_COLOR_DESC* pDesc = static_cast<BUTTON_COLOR_DESC*>(pArg);
	
	m_strTextureName = pDesc->strTextureName;
	m_vBaseColor = pDesc->vBaseColor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CButton_Color::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CButton_Color::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::LBUTTON)) && isPick(g_hWnd))
	{
		CGameManager* pGameManager = GET_GAMEMANAGER;
		pGameManager->PlaySound_Button();

		//현재 선택 되어있는 파츠의 색상을 변경한다. 현재 파츠는 게임 매니저가 관리한다.
		switch (pGameManager->Get_PartRepaint())
		{
		case Client::PART_REPAINT::BODY:
			pGameManager->Set_BodyColor(m_vBaseColor);
			break;
		case Client::PART_REPAINT::TURRET:
			pGameManager->Set_TurretColor(m_vBaseColor);
			break;
		case Client::PART_REPAINT::GUN:
			pGameManager->Set_GunColor(m_vBaseColor);
			break;
		case Client::PART_REPAINT::END:
			break;
		default:
			break;
		}


	}
}

void CButton_Color::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CButton_Color::Render()
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

HRESULT CButton_Color::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::HANGER), m_strTextureName,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CButton_Color* CButton_Color::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CButton_Color* pInstance = new CButton_Color(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CButton_Color");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton_Color::Clone(void* pArg)
{
	CButton_Color* pInstance = new CButton_Color(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CButton_Color");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton_Color::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
