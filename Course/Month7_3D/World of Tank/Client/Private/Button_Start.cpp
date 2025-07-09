#include "Button_Start.h"

#include "GameInstance.h"
#include "Level_Loading.h"

#include "GameManager.h"

CButton_Start::CButton_Start(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CButton_Start::CButton_Start(const CButton_Start& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CButton_Start::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton_Start::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CButton_Start::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CButton_Start::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_Network_Status() == NETWORK_STATUS::DISCONNECTED)
		return;

	if (m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::LBUTTON)) && isPick(g_hWnd))
	{
		if (m_bVisible == false)
			return;

		CGameManager* pGameManager = GET_GAMEMANAGER;
		pGameManager->Set_isGameStart(false);

		pGameManager->PlaySound_Button();

		pGameManager->PlayBGM_LoadingGame();

		JOIN_MATCH_DESC Desc{};
		Desc.eTank = pGameManager->Get_Select_Tank();
		Desc.iID = m_pGameInstance->Get_ID();

		Desc.vBodyColor = pGameManager->Get_BodyColor();
		Desc.vTurretColor = pGameManager->Get_TurretColor();
		Desc.vGunColor = pGameManager->Get_GunColor();
		Desc.e3DCustom = pGameManager->Get_3DCustom();

		m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_JOIN_MATCH), &Desc);

		//현재 레벨아 레벨 체인지 될거야~
		m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::GAMEPLAY));
	}
}

void CButton_Start::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CButton_Start::Render()
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

	m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), TEXT("전투 시작"), _float2(m_fX - m_fSizeX * 0.275f, m_fY - m_fSizeY * 0.275f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.5f * UI_RATIO);


	return S_OK;
}

HRESULT CButton_Start::Ready_Components()
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

CButton_Start* CButton_Start::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CButton_Start* pInstance = new CButton_Start(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CButton_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton_Start::Clone(void* pArg)
{
	CButton_Start* pInstance = new CButton_Start(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CButton_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton_Start::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
