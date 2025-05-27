#include "GameManager.h"

#include "GameInstance.h"

#include "Fury.h"
#include "Camera_TPS.h"
#include "Camera_FPS.h"
#include "SoundController.h"

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

HRESULT CGameManager::Create_My_Tank(_float3 vPos)
{
	if (FAILED(Ready_Layer_PlayerTank(TEXT("Layer_PlayerTank"), vPos)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera_FPS(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera_TPS(TEXT("Layer_Camera"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameManager::Ready_Layer_PlayerTank(const _wstring strLayerTag, _float3 vPos)
{
	CLandObject::LANDOBJECT_DESC Desc{};
	Desc.iID = m_pGameInstance->Get_ID();
	Desc.vInitPosition = vPos;

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

	//if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryTurret"),
	//	ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CGameManager::Ready_Layer_Camera_TPS(const _wstring strLayerTag)
{
	CCamera_TPS::CAMERA_TPS_DESC Desc{};
	Desc.pTarget = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_PlayerTank"))->Find_PartObject(TEXT("Part_Turret"));// ->Find_PartObject(TEXT("Part_Gun"));

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

	Desc.bActive = false;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_FPS"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameManager::PlayBGM_LoadingGame()
{
	m_pSoundCom->Play("19. Studzianki");
	m_pSoundCom->SetVolume(0.2f);
	m_pSoundCom->Set_Loop("19. Studzianki");

	return S_OK;
}

HRESULT CGameManager::StopBGM_LoadingGame()
{
	m_pSoundCom->Stop("19. Studzianki");

	return S_OK;
}

HRESULT CGameManager::PlayBGM_Game()
{
	m_pSoundCom->Play("Stuzianki_Battle");
	m_pSoundCom->SetVolume(0.2f);
	m_pSoundCom->Set_Loop("Stuzianki_Battle");

	return S_OK;
}

HRESULT CGameManager::StopBGM_Game()
{
	m_pSoundCom->Stop("Stuzianki_Battle");

	return S_OK;
}

HRESULT CGameManager::Ready_Components()
{
	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_BGM"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
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
	
	//Safe_Release(m_pSoundCom);

}
