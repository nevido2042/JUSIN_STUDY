#include "Level_MapTool.h"
#include "GameInstance.h"
#include "Level_Loading.h"

#include "Camera_Free.h"

CLevel_MapTool::CLevel_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{
	
}

HRESULT CLevel_MapTool::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Skydome(TEXT("Layer_Skydome"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MapTool(TEXT("Layer_MapTool"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_MapTool::Update(_float fTimeDelta)
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

HRESULT CLevel_MapTool::Render()
{
	SetWindowText(g_hWnd, TEXT("맵툴 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.fAmbient = 1.f;
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_Tool_Base(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tool_Base"),
		ENUM_CLASS(LEVEL::MAPTOOL), strLayerTag)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_MapTool(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_MapTool"),
		ENUM_CLASS(LEVEL::MAPTOOL), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_Skydome(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Skydome"),
		ENUM_CLASS(LEVEL::MAPTOOL), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_Terrain(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::MAPTOOL), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_Camera(const _wstring strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC Desc = {};

	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 300.0f;
	lstrcpy(Desc.szName, TEXT("Camera_Free"));

	Desc.vEye = _float3(150.f, 100.f, 100.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);
	Desc.fFov = XMConvertToRadians(60.0f);
	Desc.fNear = 0.1f;
	Desc.fFar = 500.f;

	Desc.fSensor = 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_CLASS(LEVEL::MAPTOOL), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_Fury(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Fury"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

CLevel_MapTool* CLevel_MapTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_MapTool* pInstance = new CLevel_MapTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_MapTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_MapTool::Free()
{
	__super::Free();


}
