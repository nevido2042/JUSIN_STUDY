#include "Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"

#include "BackGround.h"
#include "StatusLight.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Network(TEXT("Layer_Network"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_StatusLight(TEXT("Layer_StatusLight"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LEVEL_LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::LEVEL_GAMEPLAY))))
			return;
	}
}

HRESULT CLevel_Logo::Render()
{
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _wstring strLayerTag)
{
	CBackGround::BACKGROUND_DESC				BackGroundDesc{};

	BackGroundDesc.fX = g_iWinSizeX * 0.5f;
	BackGroundDesc.fY = g_iWinSizeY * 0.5f;
	BackGroundDesc.fSizeX = 200.0f;
	BackGroundDesc.fSizeY = 200.0f;	

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_GameObject_BackGround"),
		ENUM_CLASS(LEVEL::LEVEL_LOGO), strLayerTag, &BackGroundDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Network(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Network"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_StatusLight(const _wstring strLayerTag)
{
	CStatusLight::STATUSLIGHT_DESC				StatusLightDesc{};

	StatusLightDesc.fX = g_iWinSizeX * 0.05f;
	StatusLightDesc.fY = g_iWinSizeY * 0.05f;
	StatusLightDesc.fSizeX = 40.0f;
	StatusLightDesc.fSizeY = 50.0f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_StatusLight"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), strLayerTag, &StatusLightDesc)))
		return E_FAIL;

	return S_OK;
}


CLevel_Logo* CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Logo::Free()
{
	__super::Free();

}
