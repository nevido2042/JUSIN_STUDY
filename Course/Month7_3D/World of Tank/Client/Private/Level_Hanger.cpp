#include "Level_Hanger.h"
#include "GameInstance.h"

CLevel_Hanger::CLevel_Hanger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Hanger::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Hanger::Update(_float fTimeDelta)
{
	int a = 10;
}

HRESULT CLevel_Hanger::Render()
{
	SetWindowText(g_hWnd, TEXT("격납고 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_BackGround(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Camera(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag)))
		return E_FAIL;

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
