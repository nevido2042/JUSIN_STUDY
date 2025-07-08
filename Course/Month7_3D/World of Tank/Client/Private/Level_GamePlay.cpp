#include "Level_GamePlay.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Level_Loading.h"
#include "Camera_TPS.h"
#include "Camera_Free.h"
#include "Camera_FPS.h"
#include "GameManager.h"

#include "UIObject.h"
#include "Layer.h"
#include "Icon_Consumables.h"
#include "InvisibleWall.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
	
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Skydome(TEXT("Layer_Skydome"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Load_Map()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Boundary(TEXT("Layer_Boundary"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_InvisibleWalls(TEXT("Layer_InvisibleWall"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MapVegetation(TEXT("Layer_MapVegetation"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Camera_Free(TEXT("Layer_Camera"))))
	//	return E_FAIL;


#pragma message ("미니맵을 탱크들 모두 배치후 만들어야한다.")
	/*if (FAILED(Ready_Layer_Minimap(TEXT("Layer_Minimap"))))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_CountdownTimer(TEXT("Layer_CountdownTimer"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_DamagePanel(TEXT("Layer_DamagePanel"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Icon_Engine(TEXT("Layer_Icon_Engine"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Crosshair(TEXT("Layer_Crosshair"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_DamageBar(TEXT("Layer_DamageBar"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_GunMarker(TEXT("Layer_GunMarker"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Icon_Consumables(TEXT("Layer_Icon_Consumables"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_PickedManager(TEXT("Layer_PickedManager"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_AimCircle(TEXT("Layer_AimCircle"))))
		return E_FAIL;




	PACKET_DESC Desc = {};
	Desc.iID = m_pGameInstance->Get_ID();
	m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_LOAD_COMPLETE), &Desc);
	//m_pGameInstance->Clear_Packet();

	CGameManager* pGameManager = GET_GAMEMANAGER;
	pGameManager->StopBGM_LoadingGame();
	pGameManager->PlayBGM_Game();

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	if (m_bLevelChanged)
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, static_cast<LEVEL>(m_iNewLevelIndex)))))
			return;
	}
	else if (m_pGameInstance->Key_Down(DIK_H))
	{
		m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::HANGER));
		/*if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::HANGER))))
			return;*/
	}
	else if (m_pGameInstance->Key_Down(DIK_LSHIFT))
	{
		if (GetForegroundWindow() != g_hWnd)
			return;

		CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"));
		if (!pLayer) return;

		//카메라들 중 켜져있는거 끄고 다음꺼 킨다.
		auto& GameObjects = pLayer->Get_GameObjects();
		for (auto iter = GameObjects.begin(); iter != GameObjects.end(); ++iter)
		{
			if (true == (*iter)->Get_isActive())
			{
				CCamera* pBeforeCam = static_cast<CCamera*>(*iter);
				(*iter)->Set_Active(false);

				CCamera* pAfterCam = { nullptr };

				if (++iter != GameObjects.end())
					pAfterCam = static_cast<CCamera*>(*iter);
				else
					pAfterCam = static_cast<CCamera*>(*GameObjects.begin());

				pAfterCam->Set_Active(true);

				//1인칭 카메라와 3인칭 카메라는 yaw picth 가 반대다
				pAfterCam->Set_Yaw(-pBeforeCam->Get_Yaw() + XMConvertToRadians(-90.f));
				pAfterCam->Set_Pitch(-pBeforeCam->Get_Pitch());
				break;
			}
		}
	}
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.fAmbient = 1.f;
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	CShadow::SHADOW_DESC		Desc{};
	Desc.vEye = _float4(300.f, 200.f, 100.f, 1.f);
	Desc.vAt = _float4(300.f, 90.f, 300.f, 1.f);
	Desc.fFovy = XMConvertToRadians(60.0f);
	Desc.fNear = 0.1f;
	Desc.fFar = 500.f;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Terrain(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_MapVegetation(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_MapVegetation"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera_Free(const _wstring strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC Desc = {};

	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 100.0f;
	lstrcpy(Desc.szName, TEXT("Camera_Free"));

	Desc.vEye = _float3(150.f, 100.f, 100.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);
	Desc.fFov = XMConvertToRadians(60.0f);
	Desc.fNear = 0.1f;
	Desc.fFar = 400.f;

	Desc.fSensor = 0.1f;

	Desc.bActive = false;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Engine(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Engine"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Skydome(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Skydome"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_GunMarker(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC Desc{};
	Desc.fDepth = 0.f;// DEPTH_BACKGROUND - 0.01f;
	Desc.fSizeX = 50.f * UI_RATIO;
	Desc.fSizeY = 50.f * UI_RATIO;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_GunMarker"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_CountdownTimer(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_CountdownTimer"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Minimap(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 256.0f * UI_RATIO;
	UIObject_Desc.fSizeY = 256.0f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX - UIObject_Desc.fSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY - UIObject_Desc.fSizeY * 0.5f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Minimap"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_DamagePanel(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 229.0f * UI_RATIO;
	UIObject_Desc.fSizeY = 228.0f * UI_RATIO;
	UIObject_Desc.fX = UIObject_Desc.fSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY - UIObject_Desc.fSizeY * 0.5f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamagePanel"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

//HRESULT CLevel_GamePlay::Ready_Layer_Icon_Engine(const _wstring strLayerTag)
//{
//	CUIObject::UIOBJECT_DESC				UIObject_Desc{};
//
//	UIObject_Desc.fSizeX = 46.0f * UI_RATIO;
//	UIObject_Desc.fSizeY = 40.0f * UI_RATIO;
//	UIObject_Desc.fX = UIObject_Desc.fSizeX * 0.6f;
//	UIObject_Desc.fY = g_iWinSizeY - UIObject_Desc.fSizeY * 4.15f;
//	UIObject_Desc.fDepth = DEPTH_BACKGROUND - 0.1f;
//
//	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Icon_Engine"),
//		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &UIObject_Desc)))
//		return E_FAIL;
//
//	return S_OK;
//}

HRESULT CLevel_GamePlay::Ready_Layer_Crosshair(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 511.f * UI_RATIO;
	UIObject_Desc.fSizeY = 504.f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY * 0.5f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND - 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Crosshair"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

//HRESULT CLevel_GamePlay::Ready_Layer_DamageBar(const _wstring strLayerTag)
//{
//	CUIObject::UIOBJECT_DESC				UIObject_Desc{};
//
//	UIObject_Desc.fSizeX = 215.0f * UI_RATIO;
//	UIObject_Desc.fSizeY = 10.0f * UI_RATIO;
//	UIObject_Desc.fX = UIObject_Desc.fSizeX * 0.531f;
//	UIObject_Desc.fY = g_iWinSizeY - UIObject_Desc.fSizeY * 19.98f;
//	UIObject_Desc.fDepth = DEPTH_BACKGROUND - 0.01f;
//
//	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageBar"),
//		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &UIObject_Desc)))
//		return E_FAIL;
//
//	return S_OK;
//}

HRESULT CLevel_GamePlay::Ready_Layer_Icon_Consumables(const _wstring strLayerTag)
{
	CIcon_Consumables::ICON_CONSUMABLES_DESC				Desc{};

	Desc.fSizeX = 48.0f * UI_RATIO;
	Desc.fSizeY = 48.0f * UI_RATIO;
	Desc.fY = g_iWinSizeY * 0.95f;
	Desc.fDepth = DEPTH_BACKGROUND - 0.01f;

	Desc.strTextureName = TEXT("Prototype_Component_Texture_HandExtinguishers");
	Desc.fX = g_iWinSizeX * 0.45f;
	Desc.iKeyNumber = 4;
	Desc.eConsumables = CONSUMABLES::HAND_EXTINGUISHER;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Icon_Consumables"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	Desc.strTextureName = TEXT("Prototype_Component_Texture_SmallRepairkit");
	Desc.fX = g_iWinSizeX * 0.5f;
	Desc.iKeyNumber = 5;
	Desc.eConsumables = CONSUMABLES::SMALL_REPAIR_KIT;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Icon_Consumables"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	Desc.strTextureName = TEXT("Prototype_Component_Texture_SmallMedkit");
	Desc.fX = g_iWinSizeX * 0.55f;
	Desc.iKeyNumber = 6;
	Desc.eConsumables = CONSUMABLES::SMALL_MED_KIT;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Icon_Consumables"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_PickedManager(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_PickedManager"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Boundary(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Boundary"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_AimCircle(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC		Desc{};

	Desc.fSizeX = 2000.0f * UI_RATIO;
	Desc.fSizeY = 2000.0f * UI_RATIO;
	Desc.fDepth = DEPTH_BACKGROUND - 0.01f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_AimCircle"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_InvisibleWalls(const _wstring strLayerTag)
{
	CInvisibleWall::INVISIBLE_WALL_DESC Desc{};

	Desc.vExtents = _float3(320.f, 500.f, 1.f);
	Desc.vInitPosition = _float3(320.f, 50.f, 640.f);
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_InvisibleWall"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	Desc.vExtents = _float3(320.f, 500.f, 1.f);
	Desc.vInitPosition = _float3(320.f, 50.f, 0.f);
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_InvisibleWall"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	Desc.vExtents = _float3(1, 500.f, 320.f);
	Desc.vInitPosition = _float3(640.f, 50.f, 320.f);
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_InvisibleWall"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	Desc.vExtents = _float3(1, 500.f, 320.f);
	Desc.vInitPosition = _float3(0.f, 50.f, 320.f);
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_InvisibleWall"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Load_Map()
{
	filesystem::path MapPath = "../Bin/Map/Map.bin";

	FILE* fp = nullptr;
	fopen_s(&fp, MapPath.string().c_str(), "rb");
	if (!fp)
		return S_OK;

	while (true)
	{
		_uint iNameLength = 0;
		size_t readCount = fread(&iNameLength, sizeof(_uint), 1, fp);

		// 더 이상 읽을게 없으면 종료
		if (readCount != 1)
			break;

		// 이름 읽기
		string Name;
		Name.resize(iNameLength);
		fread(&Name[0], sizeof(char), iNameLength, fp);

		// 위치 읽기
		_float4x4 mWorldMatrix = {};
		fread(&mWorldMatrix, sizeof(_float4x4), 1, fp);

		//"Layer_Fury"
		wstring LayerName = L"Layer_" + wstring(Name.begin(), Name.end());
		//"Prototype_GameObject_Fury"
		wstring PrototypeName = L"Prototype_GameObject_" + wstring(Name.begin(), Name.end());

		m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), PrototypeName, ENUM_CLASS(LEVEL::GAMEPLAY), LayerName);
		CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), LayerName);
		if (pGameObject == nullptr)
			continue;

		// Transform 위치 설정
		CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(g_strTransformTag));
		if (pTransform != nullptr)
		{
			pTransform->Set_WorldMatrix(mWorldMatrix);
		}
	}

	fclose(fp);

	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_GamePlay::Free()
{
	__super::Free();

	//Safe_Release(m_pEngine_Sound_Tool);

}
