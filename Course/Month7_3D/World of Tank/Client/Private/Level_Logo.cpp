#include "Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"

#include "UIObject.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Logo(TEXT("Layer_Logo"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button_Login(TEXT("Layer_Button_Login"))))
		return E_FAIL;



	//스태틱
	if (FAILED(Ready_Layer_Tool_Base(TEXT("Layer_Tool_Base"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_StatusLight(TEXT("Layer_StatusLight"))))
		return E_FAIL;

	return S_OK;
 }

void CLevel_Logo::Update(_float fTimeDelta)
{
	if (m_bLevelChanged)
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, static_cast<LEVEL>(m_iNewLevelIndex)))))
			return;
	}
	else if (m_pGameInstance->Key_Down(DIK_RETURN))
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::HANGER))))
			return;
	}
	else if (m_pGameInstance->Key_Down(DIK_T)/*m_pGameInstance->Key_Down(DIK_F3)*/)
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::MAPTOOL))))
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
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fX = g_iWinSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY * 0.5f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND;
	UIObject_Desc.fSizeX = static_cast<_float>(g_iWinSizeX);
	UIObject_Desc.fSizeY = static_cast<_float>(g_iWinSizeY);

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		ENUM_CLASS(LEVEL::LOGO), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Network(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Network"),
		ENUM_CLASS(LEVEL::STATIC), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_StatusLight(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fX = g_iWinSizeX * 0.05f;
	UIObject_Desc.fY = g_iWinSizeY * 0.05f;
#pragma message ("UI 크기도 WinSize 비례해서 하면 좋겠지만. 툴 없이는 힘들지도, 왜냐하면 화면비율 망가진 상태에서 생성하면 비율 망가짐")
	UIObject_Desc.fSizeX = 40.0f;
	UIObject_Desc.fSizeY = 50.0f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_StatusLight"),
		ENUM_CLASS(LEVEL::STATIC), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Logo(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fX = g_iWinSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY * 0.5f;
	UIObject_Desc.fSizeX = 400.f;
	UIObject_Desc.fSizeY = 200.f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo"),
		ENUM_CLASS(LEVEL::LOGO), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Button_Login(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fX = g_iWinSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY * 0.8f;
	UIObject_Desc.fSizeX = 200.f;
	UIObject_Desc.fSizeY = 50.f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Button_Login"),
		ENUM_CLASS(LEVEL::LOGO), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Tool_Base(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tool_Base"),
		ENUM_CLASS(LEVEL::STATIC), strLayerTag)))
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
