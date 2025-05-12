#include "Level_Practice.h"

#include "GameInstance.h"

CLevel_Practice::CLevel_Practice(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{

}

HRESULT CLevel_Practice::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Engine(TEXT("Layer_Engine"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Fury(TEXT("Layer_Fury"))))
		return E_FAIL;


	if (FAILED(Ready_Layer_Tool_Base(TEXT("Layer_Tool_Base"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Tool_EngineSound(TEXT("Layer_Tool_EngineSound"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Practice::Update(_float fTimeDelta)
{
	//m_pEngine_Sound_Tool->Update(fTimeDelta);
}

HRESULT CLevel_Practice::Render()
{
	SetWindowText(g_hWnd, TEXT("연습 레벨입니다."));
	//m_pEngine_Sound_Tool->Render();
	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_BackGround(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Practice::Ready_Layer_Camera(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_CLASS(LEVEL::PRACTICE), strLayerTag)))
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
