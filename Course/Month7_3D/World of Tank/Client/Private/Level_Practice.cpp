#include "Level_Practice.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Level_Loading.h"
#include "Camera_TPS.h"
#include "Camera_Free.h"
#include "Camera_FPS.h"
#include "GameManager.h"

#include "LandObject.h"
#include "UIObject.h"
#include "Layer.h"
#include "Terrain.h"


CLevel_Practice::CLevel_Practice(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{

}

HRESULT CLevel_Practice::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Skydome(TEXT("Layer_Skydome"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Load_Map()))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Engine(TEXT("Layer_Engine"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_PlayerTank(TEXT("Layer_PlayerTank"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_DummyTank(TEXT("Layer_Tank"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Camera_Free(TEXT("Layer_Camera"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Camera_FPS(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera_TPS(TEXT("Layer_Camera"))))
		return E_FAIL;


	if (FAILED(Ready_Layer_Minimap(TEXT("Layer_Minimap"))))
		return E_FAIL;

	
	if (FAILED(Ready_Layer_DamagePanel(TEXT("Layer_DamagePanel"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Icon_Engine(TEXT("Layer_Icon_Engine"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Crosshair(TEXT("Layer_Crosshair"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_DamageBar(TEXT("Layer_DamageBar"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_GunMarker(TEXT("Layer_GunMarker"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_DirectionBody(TEXT("Layer_DirectionBody"))))
		return E_FAIL;



	//if (FAILED(Ready_Layer_PersonalArrowEntry(TEXT("Layer_PersonalArrowEntry"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Tool_EngineSound(TEXT("Layer_Tool_EngineSound"))))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_Practice::Update(_float fTimeDelta)
{
	if (m_bLevelChanged)
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, static_cast<LEVEL>(m_iNewLevelIndex)))))
			return;
	}
	else if (m_pGameInstance->Key_Down(DIK_H))
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::HANGER))))
			return;
	}
	else if (m_pGameInstance->Key_Down(DIK_LSHIFT))
	{
		if (GetForegroundWindow() != g_hWnd)
			return;

		CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::PRACTICE), TEXT("Layer_Camera"));
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

HRESULT CLevel_Practice::Render()
{
	SetWindowText(g_hWnd, TEXT("연습 레벨입니다."));
	//m_pEngine_Sound_Tool->Render();
	return S_OK;
}

HRESULT CLevel_Practice::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_Terrain(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_Camera_Free(const _wstring strLayerTag)
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
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_Camera_TPS(const _wstring strLayerTag)
{
	CCamera_TPS::CAMERA_TPS_DESC Desc{};
	Desc.pTarget = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::PRACTICE), TEXT("Layer_PlayerTank"))->Find_PartObject(TEXT("Part_Turret"));
	Desc.bActive = true;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_TPS"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_Camera_FPS(const _wstring strLayerTag)
{
	CCamera_FPS::CAMERA_FPS_DESC Desc{};
	Desc.pTarget = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::PRACTICE), TEXT("Layer_PlayerTank"))->Find_PartObject(TEXT("Part_Turret"));
	Desc.bActive = false;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_FPS"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_Engine(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Engine"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_Skydome(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Skydome"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_PlayerTank(const _wstring strLayerTag)
{
	TANK eSelectTank = static_cast<CGameManager*>(m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Layer_GameManager")))->Get_Select_Tank();

	CLandObject::LANDOBJECT_DESC Desc{};
	Desc.vInitPosition = _float3(128.f * 5.f * 0.5f, 0.f, 128.f * 5.f * 0.5f);
	Desc.fRotationPerSec = 1.f;
	Desc.fSpeedPerSec = 1.f;
	Desc.iID = m_pGameInstance->Get_ID();
	
	switch (eSelectTank)
	{
	case TANK::FURY:
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Fury"),
			ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &Desc)))
			return E_FAIL;
		break;
	case TANK::TIGER:
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tiger"),
			ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &Desc)))
			return E_FAIL;
		break;
	case TANK::END:
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_DummyTank(const _wstring strLayerTag)
{
	CLandObject::LANDOBJECT_DESC Desc{};
	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;

	Desc.vInitPosition = _float3(150.f, 87.f, 200.f);
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Fury"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &Desc)))
		return E_FAIL;

	Desc.vInitPosition = _float3(250.f, 87.f, 200.f);
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tiger"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_GunMarker(const _wstring strLayerTag)
{
	CGameObject::GAMEOBJECT_DESC Desc{};
	Desc.vInitPosition = _float3(100.f, 110.f, 100.f);
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_GunMarker"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_Minimap(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 256.0f * 1.5f * UI_RATIO;
	UIObject_Desc.fSizeY = 256.0f * 1.5f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX - UIObject_Desc.fSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY - UIObject_Desc.fSizeY * 0.5f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Minimap"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_DamagePanel(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 229.0f * UI_RATIO;
	UIObject_Desc.fSizeY = 228.0f * UI_RATIO;
	UIObject_Desc.fX = UIObject_Desc.fSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY - UIObject_Desc.fSizeY * 0.5f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamagePanel"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_Icon_Engine(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 46.0f * UI_RATIO;
	UIObject_Desc.fSizeY = 40.0f * UI_RATIO;
	UIObject_Desc.fX = UIObject_Desc.fSizeX * 0.6f;
	UIObject_Desc.fY = g_iWinSizeY - UIObject_Desc.fSizeY * 4.15f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND - 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Icon_Engine"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_PersonalArrowEntry(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 188.0f * UI_RATIO;
	UIObject_Desc.fSizeY = 226.0f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY * 0.5f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND - 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_PersonalArrowEntry"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_Crosshair(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 511.f * UI_RATIO;
	UIObject_Desc.fSizeY = 504.f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY * 0.5f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND - 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Crosshair"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_DamageBar(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 215.0f * UI_RATIO;
	UIObject_Desc.fSizeY = 10.0f * UI_RATIO;
	UIObject_Desc.fX = UIObject_Desc.fSizeX * 0.531f;
	UIObject_Desc.fY = g_iWinSizeY - UIObject_Desc.fSizeY * 19.98f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND - 0.01f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageBar"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_DirectionBody(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 48.0f * UI_RATIO;
	UIObject_Desc.fSizeY = 74.0f * UI_RATIO;
	UIObject_Desc.fX = UIObject_Desc.fSizeX * 2.35f;
	UIObject_Desc.fY = g_iWinSizeY - UIObject_Desc.fSizeY * 1.3f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND - 0.01f;
	UIObject_Desc.fRotationPerSec = 1.f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DirectionBody"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Load_Map()
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

		m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), PrototypeName, ENUM_CLASS(LEVEL::PRACTICE), LayerName);
		CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::PRACTICE), LayerName);
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

CLevel_Practice* CLevel_Practice::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Practice* pInstance = new CLevel_Practice(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Practice");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Practice::Free()
{
	__super::Free();

	//Safe_Release(m_pEngine_Sound_Tool);

}
