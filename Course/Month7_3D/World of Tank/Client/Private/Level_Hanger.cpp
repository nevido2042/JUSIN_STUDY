#include "Level_Hanger.h"

#include "GameInstance.h"
#include "Level_Loading.h"

#include "GameObject.h"
#include "Camera_Free.h"

CLevel_Hanger::CLevel_Hanger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Hanger::Initialize()
{
	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Load_Map()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Hanger::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_P))
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::PRACTICE))))
			return;
	}
	else if (m_pGameInstance->Key_Down(DIK_T)/*m_pGameInstance->Key_Down(DIK_F3)*/)
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::MAPTOOL))))
			return;
	}
}

HRESULT CLevel_Hanger::Render()
{
	SetWindowText(g_hWnd, TEXT("격납고 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Terrain(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Camera(const _wstring strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC Desc = {};

	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 300.0f;
	lstrcpy(Desc.szName, TEXT("Camera_Free"));

	Desc.vEye = _float3(300.f, 100.f, 480.f);
	Desc.vAt = _float3(383.f, 87.f, 481.f);
	Desc.fFov = XMConvertToRadians(60.0f);
	Desc.fNear = 0.1f;
	Desc.fFar = 4000.f;

	Desc.fSensor = 0.1f;

	//Desc.vInitPosition = { 1000.f, 1000.f, 1000.f };

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Load_Map()
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

#pragma message("만들어질 오브젝트에게 클론될 레벨인덱스를 전달하자: Fury 가 터레인을 못찾음")
		
		CGameObject::GAMEOBJECT_DESC Desc = {};
		Desc.iLevelIndex = ENUM_CLASS(LEVEL::HANGER);

		m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), PrototypeName, ENUM_CLASS(LEVEL::HANGER), LayerName, &Desc);
		CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::HANGER), LayerName);
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

CLevel_Hanger* CLevel_Hanger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Hanger* pInstance = new CLevel_Hanger(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Hanger");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Hanger::Free()
{
	__super::Free();

}
