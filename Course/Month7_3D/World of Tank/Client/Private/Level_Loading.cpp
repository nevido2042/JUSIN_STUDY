#include "Level_Loading.h"

#include "Level_Logo.h"
#include "Level_Hanger.h"
#include "Level_Practice.h"
#include "Level_GamePlay.h"
#include "Level_MapTool.h"

#include "Loader.h"

#include "BackGround.h"

#include "GameInstance.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	/* 로딩레벨 자체에 필요한 객체를 생성한다. */
	/* 배경, 로딩바, 버튼, font */

	/* 로딩의 역할(다음레벨에 필요한 자원(Resource)(텍스쳐, 모델, 사운드 등등등 )을 생성하는)을 
	수행할 로더객체를 생성한다. */
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (FAILED(Ready_Layer_Background(TEXT("Layer_Background"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Loading_Spinner(TEXT("Layer_Loading_Spinner"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_WOT_Icon(TEXT("Layer_Loading_Spinner"))))
		return E_FAIL;

	//Ready_Layer_Background(TEXT("Layer_Background"));
	//Ready_Layer_Loading_Spinner(TEXT("Layer_Loading_Spinner"));
	
	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{

	//if (GetKeyState(VK_SPACE) & 0x8000)
	//{
		if (true == m_pLoader->isFinished())
		{
			CLevel* pLevel = { nullptr };

			switch (m_eNextLevelID)
			{
			case LEVEL::LOGO:
				pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL::HANGER:
				pLevel = CLevel_Hanger::Create(m_pDevice, m_pContext);
				break;
			case LEVEL::PRACTICE:
				pLevel = CLevel_Practice::Create(m_pDevice, m_pContext);
				break;
			case LEVEL::GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			case LEVEL::MAPTOOL:
				pLevel = CLevel_MapTool::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pLevel)
				return;

			if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(m_eNextLevelID), pLevel)))
				return;
							
		}
	//}	
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Output_LoadingText();

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_Background(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObjectDesc{};

	UIObjectDesc.fX = g_iWinSizeX * 0.5f;
	UIObjectDesc.fY = g_iWinSizeY * 0.5f;
	UIObjectDesc.fDepth = DEPTH_BACKGROUND;
	UIObjectDesc.fSizeX = static_cast<_float>(g_iWinSizeX);
	UIObjectDesc.fSizeY = static_cast<_float>(g_iWinSizeY);

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Background_Loading"),
		ENUM_CLASS(LEVEL::LOADING), strLayerTag, &UIObjectDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_Loading_Spinner(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObjectDesc{};
	UIObjectDesc.fRotationPerSec = 1.f;
	UIObjectDesc.fX = g_iWinSizeX * 0.5f;
	UIObjectDesc.fY = g_iWinSizeY * 0.5f;
	UIObjectDesc.fSizeX = 260.0f * UI_RATIO;
	UIObjectDesc.fSizeY = 260.0f * UI_RATIO;
	UIObjectDesc.fDepth = 0.05f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Loading_Spinner"),
		ENUM_CLASS(LEVEL::LOADING), strLayerTag, &UIObjectDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_WOT_Icon(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObjectDesc{};
	UIObjectDesc.fX = g_iWinSizeX * 0.5f;
	UIObjectDesc.fY = g_iWinSizeY * 0.5f;
	UIObjectDesc.fSizeX = 260.0f * UI_RATIO;
	UIObjectDesc.fSizeY = 260.0f * UI_RATIO;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_WOT_Icon"),
		ENUM_CLASS(LEVEL::LOADING), strLayerTag, &UIObjectDesc)))
		return E_FAIL;

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
