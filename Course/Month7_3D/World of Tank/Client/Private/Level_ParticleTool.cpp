#include "Level_ParticleTool.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Free.h"

CLevel_ParticleTool::CLevel_ParticleTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CLevel(pDevice, pContext)
{

}

HRESULT CLevel_ParticleTool::Initialize()
{
	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Load_Map()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera_Free(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Skydome(TEXT("Layer_Skydome"))))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_ParticleTool(TEXT("Layer_ParticleTool"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_ParticleTool::Update(_float fTimeDelta)
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
}

HRESULT CLevel_ParticleTool::Render()
{
	SetWindowText(g_hWnd, TEXT("파티클 툴 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_ParticleTool::Ready_Layer_Terrain(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::PARTICLETOOL), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_ParticleTool::Ready_Layer_Camera_Free(const _wstring strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC Desc = {};

	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 10.0f;
	lstrcpy(Desc.szName, TEXT("Camera_Free"));

	Desc.vEye = _float3(300.f, 100.f, 300.f);
	Desc.vAt = _float3(0.f, 0.f, 1.f);
	Desc.fFov = XMConvertToRadians(60.0f);
	Desc.fNear = 0.1f;
	Desc.fFar = 4000.f;

	Desc.fSensor = 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_CLASS(LEVEL::PARTICLETOOL), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_ParticleTool::Load_Map()
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

		m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), PrototypeName, ENUM_CLASS(LEVEL::PARTICLETOOL), LayerName);
		CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::PARTICLETOOL), LayerName);
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

HRESULT CLevel_ParticleTool::Ready_Layer_Skydome(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Skydome"),
		ENUM_CLASS(LEVEL::PARTICLETOOL), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_ParticleTool::Ready_Lights()
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

HRESULT CLevel_ParticleTool::Ready_Layer_Effect(const _wstring strLayerTag)
{
	CGameObject::GAMEOBJECT_DESC Desc{};
	Desc.vInitPosition = _float3(300.f, 100.f, 300.f);

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Smoke"),
		ENUM_CLASS(LEVEL::PARTICLETOOL), strLayerTag, &Desc)))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Ash"),
	//	ENUM_CLASS(LEVEL::PARTICLETOOL), strLayerTag, &Desc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_ParticleTool::Ready_Layer_ParticleTool(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::PARTICLETOOL), TEXT("Prototype_GameObject_ParticleTool"),
		ENUM_CLASS(LEVEL::PARTICLETOOL), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

CLevel_ParticleTool* CLevel_ParticleTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_ParticleTool* pInstance = new CLevel_ParticleTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_ParticleTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_ParticleTool::Free()
{
	__super::Free();
}
