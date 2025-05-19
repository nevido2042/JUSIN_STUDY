#include "Level_Practice.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Level_Loading.h"
#include "Camera_TPS.h"
#include "Camera_Free.h"

#include "UIObject.h"

CLevel_Practice::CLevel_Practice(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{

}

HRESULT CLevel_Practice::Initialize()
{
	if (FAILED(Ready_Layer_Skydome(TEXT("Layer_Skydome"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Load_Map()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera_TPS(TEXT("Layer_Camera_TPS"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Minimap(TEXT("Layer_Minimap"))))
		return E_FAIL;

	
	if (FAILED(Ready_Layer_DamagePanel(TEXT("Layer_DamagePanel"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_Camera_Free(TEXT("Layer_Camera_Free"))))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_Engine(TEXT("Layer_Engine"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_Fury(TEXT("Layer_Fury"))))
		return E_FAIL;*/
	
	//if (FAILED(Ready_Layer_Tool_Base(TEXT("Layer_Tool_Base"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Tool_EngineSound(TEXT("Layer_Tool_EngineSound"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Practice::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_H))
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::HANGER))))
			return;
	}
}

HRESULT CLevel_Practice::Render()
{
	SetWindowText(g_hWnd, TEXT("연습 레벨입니다."));
	//m_pEngine_Sound_Tool->Render();
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
	Desc.fSpeedPerSec = 300.0f;
	lstrcpy(Desc.szName, TEXT("Camera_Free"));

	Desc.vEye = _float3(150.f, 100.f, 100.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);
	Desc.fFov = XMConvertToRadians(60.0f);
	Desc.fNear = 0.1f;
	Desc.fFar = 4000.f;

	Desc.fSensor = 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_Camera_TPS(const _wstring strLayerTag)
{
	CCamera_TPS::CAMERA_TPS_DESC Desc{};
	Desc.pTarget = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::PRACTICE),TEXT("Layer_Fury"));

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_TPS"),
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

HRESULT CLevel_Practice::Ready_Layer_Fury(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Fury"),
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

HRESULT CLevel_Practice::Ready_Layer_Minimap(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 256.0f;
	UIObject_Desc.fSizeY = 256.0f;
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

	UIObject_Desc.fSizeX = 229.0f;
	UIObject_Desc.fSizeY = 228.0f;
	UIObject_Desc.fX = UIObject_Desc.fSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY - UIObject_Desc.fSizeY * 0.5f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamagePanel"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

//HRESULT CLevel_Practice::Ready_Layer_Tool_Base(const _wstring strLayerTag)
//{
//	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tool_Base"),
//		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag)))
//		return E_FAIL;
//
//	return S_OK;
//}

HRESULT CLevel_Practice::Ready_Layer_Tool_Base(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tool_Base"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_Tool_EngineSound(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::PRACTICE), TEXT("Prototype_GameObject_Tool_EngineSound"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag)))
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

		CGameObject::GAMEOBJECT_DESC Desc = {};
		Desc.iLevelIndex = ENUM_CLASS(LEVEL::PRACTICE);

		m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), PrototypeName, ENUM_CLASS(LEVEL::PRACTICE), LayerName, &Desc);
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
