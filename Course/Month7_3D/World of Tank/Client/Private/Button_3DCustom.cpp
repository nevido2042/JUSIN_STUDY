#include "Button_3DCustom.h"

#include "GameInstance.h"
#include "GameManager.h"

CButton_3DCustom::CButton_3DCustom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CButton_3DCustom::CButton_3DCustom(const CButton_3DCustom& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CButton_3DCustom::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton_3DCustom::Initialize(void* pArg)
{
	BUTTON_3DCUSTOM_DESC* pDesc = static_cast<BUTTON_3DCUSTOM_DESC*>(pArg);

	m_strTextureName = pDesc->strTextureName;
	m_e3DCustom = pDesc->e3DCustom;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CButton_3DCustom::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CButton_3DCustom::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::LBUTTON)) && isPick(g_hWnd))
	{
		if (m_bVisible == false)
			return;

		CGameManager* pGameManager = GET_GAMEMANAGER;
		pGameManager->PlaySound_Button();

		CGameObject* pFury = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Layer_Fury_Hanger"));
		CGameObject* pTiger = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Layer_Tiger_Hanger"));
		
		CGameObject* pFuryTurret = pFury->Find_PartObject(TEXT("Part_Turret"));
		CGameObject* pTigerTurret = pTiger->Find_PartObject(TEXT("Part_Turret"));

#pragma region ´Ù¸¥°Å ´Ù²û
		pFuryTurret->Find_PartObject(TEXT("Part_Monkey"))->Set_Visible(false);
		pTigerTurret->Find_PartObject(TEXT("Part_Monkey"))->Set_Visible(false);

		pFuryTurret->Find_PartObject(TEXT("Part_DragonHead"))->Set_Visible(false);
		pTigerTurret->Find_PartObject(TEXT("Part_DragonHead"))->Set_Visible(false);

		pFuryTurret->Find_PartObject(TEXT("Part_Drum"))->Set_Visible(false);
		pTigerTurret->Find_PartObject(TEXT("Part_Drum"))->Set_Visible(false);

		pFuryTurret->Find_PartObject(TEXT("Part_Speaker"))->Set_Visible(false);
		pTigerTurret->Find_PartObject(TEXT("Part_Speaker"))->Set_Visible(false);

		pFuryTurret->Find_PartObject(TEXT("Part_Guitar"))->Set_Visible(false);
		pTigerTurret->Find_PartObject(TEXT("Part_Guitar"))->Set_Visible(false);

		pFuryTurret->Find_PartObject(TEXT("Part_Shark"))->Set_Visible(false);
		pTigerTurret->Find_PartObject(TEXT("Part_Shark"))->Set_Visible(false);

		pFuryTurret->Find_PartObject(TEXT("Part_GoldPot"))->Set_Visible(false);
		pTigerTurret->Find_PartObject(TEXT("Part_GoldPot"))->Set_Visible(false);
#pragma endregion

		switch (m_e3DCustom)
		{
		case CUSTOM3D::MONKEY:
			pFuryTurret->Find_PartObject(TEXT("Part_Monkey"))->Set_Visible(true);
			pTigerTurret->Find_PartObject(TEXT("Part_Monkey"))->Set_Visible(true);
			pGameManager->Set_3DCustom(CUSTOM3D::MONKEY);
			break;

		case CUSTOM3D::DRAGONHEAD:
			pFuryTurret->Find_PartObject(TEXT("Part_DragonHead"))->Set_Visible(true);
			pTigerTurret->Find_PartObject(TEXT("Part_DragonHead"))->Set_Visible(true);
			pGameManager->Set_3DCustom(CUSTOM3D::DRAGONHEAD);
			break;

		case CUSTOM3D::DRUM:
			pFuryTurret->Find_PartObject(TEXT("Part_Drum"))->Set_Visible(true);
			pTigerTurret->Find_PartObject(TEXT("Part_Drum"))->Set_Visible(true);
			pGameManager->Set_3DCustom(CUSTOM3D::DRUM);
			break;

		case CUSTOM3D::SPEAKER:
			pFuryTurret->Find_PartObject(TEXT("Part_Speaker"))->Set_Visible(true);
			pTigerTurret->Find_PartObject(TEXT("Part_Speaker"))->Set_Visible(true);
			pGameManager->Set_3DCustom(CUSTOM3D::SPEAKER);
			break;

		case CUSTOM3D::GUITAR:
			pFuryTurret->Find_PartObject(TEXT("Part_Guitar"))->Set_Visible(true);
			pTigerTurret->Find_PartObject(TEXT("Part_Guitar"))->Set_Visible(true);
			pGameManager->Set_3DCustom(CUSTOM3D::GUITAR);
			break;

		case CUSTOM3D::SHARK:
			pFuryTurret->Find_PartObject(TEXT("Part_Shark"))->Set_Visible(true);
			pTigerTurret->Find_PartObject(TEXT("Part_Shark"))->Set_Visible(true);
			pGameManager->Set_3DCustom(CUSTOM3D::SHARK);
			break;

		case CUSTOM3D::GOLDPOT:
			pFuryTurret->Find_PartObject(TEXT("Part_GoldPot"))->Set_Visible(true);
			pTigerTurret->Find_PartObject(TEXT("Part_GoldPot"))->Set_Visible(true);
			pGameManager->Set_3DCustom(CUSTOM3D::GOLDPOT);
			break;

		case CUSTOM3D::END:
			pGameManager->Set_3DCustom(CUSTOM3D::END);
			break;
		}

		

	}
}

void CButton_3DCustom::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CButton_3DCustom::Render()
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

HRESULT CButton_3DCustom::Ready_Components()
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

CButton_3DCustom* CButton_3DCustom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CButton_3DCustom* pInstance = new CButton_3DCustom(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CButton_3DCustom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton_3DCustom::Clone(void* pArg)
{
	CButton_3DCustom* pInstance = new CButton_3DCustom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CButton_3DCustom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton_3DCustom::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
