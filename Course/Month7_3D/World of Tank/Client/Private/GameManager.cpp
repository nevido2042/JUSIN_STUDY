#include "GameManager.h"

#include "GameInstance.h"

#include "Camera_TPS.h"
#include "Camera_FPS.h"
#include "SoundController.h"
#include "Tank.h"
#include "Layer.h"
#include "Button_Part.h"

CGameManager::CGameManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CGameManager::CGameManager(const CGameManager& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CGameManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameManager::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};

	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("GameManager"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pSoundCom_GUI->SetVolume(0.3f);

	return S_OK;
}

void CGameManager::Priority_Update(_float fTimeDelta)
{

}

void CGameManager::Update(_float fTimeDelta)
{
		
}

void CGameManager::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CGameManager::Render()
{

	return S_OK;
}

HRESULT CGameManager::Create_My_Tank(SPAWN_TANK_DESC* pDesc)
{
	if (FAILED(Ready_Layer_PlayerTank(TEXT("Layer_PlayerTank"), pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera_FPS(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera_TPS(TEXT("Layer_Camera"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameManager::Ready_Layer_PlayerTank(const _wstring strLayerTag, SPAWN_TANK_DESC* pDesc)
{
	CTank::TANK_DESC Desc{};
	Desc.iID = m_pGameInstance->Get_ID();
	Desc.fRotationPerSec = 1.f;
	Desc.fSpeedPerSec = 1.f;
	Desc.vInitPosition = pDesc->vPos;
	Desc.eTeam = pDesc->eTeam;

	Desc.vBodyColor = m_vBodyColor;
	Desc.vTurretColor = m_vTurretColor;
	Desc.vGunColor = m_vGunColor;
	Desc.e3DCustom = m_e3DCustom;

	switch (m_eSelectTank)
	{
	case TANK::FURY:
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Fury"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
			return E_FAIL;
		break;
	case TANK::TIGER:
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tiger"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
			return E_FAIL;
		break;
	case TANK::END:
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CGameManager::Ready_Layer_Camera_TPS(const _wstring strLayerTag)
{
	CCamera_TPS::CAMERA_TPS_DESC Desc{};
	Desc.pTarget = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_PlayerTank"))->Find_PartObject(TEXT("Part_Turret"));// ->Find_PartObject(TEXT("Part_Gun"));
	lstrcpy(Desc.szName, TEXT("Camera_TPS"));
	Desc.bActive = true;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_TPS"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameManager::Ready_Layer_Camera_FPS(const _wstring strLayerTag)
{
	CCamera_FPS::CAMERA_FPS_DESC Desc{};
	Desc.pTarget = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_PlayerTank"))->Find_PartObject(TEXT("Part_Turret"));// ->Find_PartObject(TEXT("Part_Gun"));
	lstrcpy(Desc.szName, TEXT("Camera_FPS"));
	Desc.bActive = false;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_FPS"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameManager::PlayBGM_LoadingGame()
{
	m_pSoundCom_BGM->Play("19. Studzianki");
	m_pSoundCom_BGM->SetVolume(0.2f);
	m_pSoundCom_BGM->Set_Loop("19. Studzianki");

	return S_OK;
}

HRESULT CGameManager::StopBGM_LoadingGame()
{
	m_pSoundCom_BGM->Stop("19. Studzianki");

	return S_OK;
}

HRESULT CGameManager::PlayBGM_Game()
{
	if(m_pSoundCom_BGM->IsPlaying("Stuzianki_Battle"))
		m_pSoundCom_BGM->Stop("Stuzianki_Battle");

	m_pSoundCom_BGM->Play("Stuzianki_Battle");
	m_pSoundCom_BGM->SetVolume(0.2f);
	m_pSoundCom_BGM->Set_Loop("Stuzianki_Battle");

	return S_OK;
}

HRESULT CGameManager::StopBGM_Game()
{
	m_pSoundCom_BGM->Stop("Stuzianki_Battle");

	return S_OK;
}

HRESULT CGameManager::PlaySound_Button()
{
	m_pSoundCom_GUI->Play("gui_25");
	return S_OK;
}

HRESULT CGameManager::PlaySound_Repair()
{
	m_pSoundCom_GUI->Play("consumables_18");
	return S_OK;
}

HRESULT CGameManager::PlaySound_Medkit()
{
	m_pSoundCom_GUI->Play("consumables_15");
	return S_OK;
}

HRESULT CGameManager::PlaySound_Extinguisher()
{
	m_pSoundCom_GUI->Play("consumables_5");
	return S_OK;
}

void CGameManager::Set_PartRepaint(const PART_REPAINT& ePartRepaint)
{
	//해당 버튼을 활성화한다. 나머지는 다 끄고
	m_ePartRepaint = ePartRepaint;

	CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::HANGER), TEXT("Layer_Button_Customize"));

	if (pLayer == nullptr)
	{
		return;
	}

	switch (m_ePartRepaint)
	{
	case PART_REPAINT::BODY:
		static_cast<CButton_Part*>(pLayer->Find_GameObject_By_Name(TEXT("Button_Repaint_Body")))->Enable_Highlight();
		static_cast<CButton_Part*>(pLayer->Find_GameObject_By_Name(TEXT("Button_Repaint_Turret")))->Disable_Highlight();
		static_cast<CButton_Part*>(pLayer->Find_GameObject_By_Name(TEXT("Button_Repaint_Gun")))->Disable_Highlight();
		break;
	case PART_REPAINT::TURRET:
		static_cast<CButton_Part*>(pLayer->Find_GameObject_By_Name(TEXT("Button_Repaint_Turret")))->Enable_Highlight();
		static_cast<CButton_Part*>(pLayer->Find_GameObject_By_Name(TEXT("Button_Repaint_Body")))->Disable_Highlight();
		static_cast<CButton_Part*>(pLayer->Find_GameObject_By_Name(TEXT("Button_Repaint_Gun")))->Disable_Highlight();
		break;
	case PART_REPAINT::GUN:
		static_cast<CButton_Part*>(pLayer->Find_GameObject_By_Name(TEXT("Button_Repaint_Gun")))->Enable_Highlight();
		static_cast<CButton_Part*>(pLayer->Find_GameObject_By_Name(TEXT("Button_Repaint_Body")))->Disable_Highlight();
		static_cast<CButton_Part*>(pLayer->Find_GameObject_By_Name(TEXT("Button_Repaint_Turret")))->Disable_Highlight();
		break;
	}

}

HRESULT CGameManager::Ready_Components()
{
	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_BGM"),
		TEXT("Com_Sound_BGM"), reinterpret_cast<CComponent**>(&m_pSoundCom_BGM))))
		return E_FAIL;

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_GUI"),
		TEXT("Com_Sound_GUI"), reinterpret_cast<CComponent**>(&m_pSoundCom_GUI))))
		return E_FAIL;

	return S_OK;
}

CGameManager* CGameManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGameManager* pInstance = new CGameManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGameManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGameManager::Clone(void* pArg)
{
	CGameManager* pInstance = new CGameManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGameManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameManager::Free()
{
	__super::Free();
	
	Safe_Release(m_pSoundCom_GUI);
	Safe_Release(m_pSoundCom_BGM);

}
