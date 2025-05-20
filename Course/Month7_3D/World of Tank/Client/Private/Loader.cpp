#include "Loader.h"

#include "GameInstance.h"
//#include "player.h"
//#include "Effect.h"
//#include "Sky.h"
#pragma region LODING
#include "Level_Loading.h"
#include "BackGround_Loading.h"
#include "Loading_Spinner.h"
#pragma endregion

#pragma region STATIC
#include "GameManager.h"
#include "Button_Start.h"
#include "DamagePanel.h"
#include "Minimap.h"
#include "StatusLight.h"
#include "ChurchA.h"
#include "BurntTree.h"
#include "VHouse01A.h"
#include "Fury.h"
#include "Tiger.h"
#include "Skydome.h"
#include "Camera_Free.h"
#include "Camera_TPS.h"
#include "Terrain.h"
#include "Engine.h"
#include "Tool_Base.h"
#pragma endregion

#pragma region LOGO
#include "BackGround.h"
#include "Network.h"
#include "Logo.h"
#include "Button_Login.h"
#pragma endregion

#pragma region HANGER
#include "Fury_Hanger.h"
#include "Button_Fury.h"
#include "Tiger_Hanger.h"
#include "Button_Tiger.h"
#pragma endregion

#pragma region MAPTOOL
#include "MapTool.h"
#pragma endregion

#pragma region PRACTICE
#include "Tool_Engine_Sound.h"
#include "MapObject.h"
#pragma endregion

#pragma region GAMEPLAY

#pragma endregion



_bool CLoader::bLoadStatic = { false };

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	/* 자원로딩한다. */
	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;	

	return 0;

}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	CoInitializeEx(nullptr, 0);	

	EnterCriticalSection(&m_CriticalSection);

	HRESULT		hr = {};

	//스태틱 프로토타입을 만들었는가?
	if (!bLoadStatic)
	{
		hr = Loading_For_Static();
		if (FAILED(hr))
			return E_FAIL;
	}

	switch (m_eNextLevelID)
	{
	case LEVEL::LOGO:
		hr = Loading_For_Logo();
		break;

	case LEVEL::HANGER:
		hr = Loading_For_Hanger();
		break;

	case LEVEL::PRACTICE:
		hr = Loading_For_Practice();
		break;

	case LEVEL::GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;

	case LEVEL::MAPTOOL:
		hr = Loading_For_MapTool();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);



	return S_OK;
}



HRESULT CLoader::Loading_For_Static()
{


#pragma region 텍스쳐
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	/* For.Prototype_Component_Texture_Button_Login */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Button_Login"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/components/output/back_primary_red.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/Map/99_poland/spaces/99_poland/global_AM.dds"), 1))))
		return E_FAIL;
	//"../Bin/WOT_Resources/Map/99_poland/spaces/99_poland/global_AM.dds"
	//"../Bin/Resources/Textures/Terrain/Tile0.dds"

		/* For.Prototype_Component_Texture_StatusLight*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_StatusLight"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/00.StatusLight/StatusLight%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Minimap */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Minimap"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/Map/99_poland/spaces/99_poland/mmap.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_DamagePanel */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DamagePanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/battleAtlas/output/DamagePanel_bg.dds"), 1))))
		return E_FAIL;

#pragma endregion

#pragma region 모델
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/Map/99_poland/spaces/99_poland/outland/height_map_cut_128.png"), {5.f, 1.f}))))
		return E_FAIL;

	//height_map_cut_128
	//"../Bin/WOT_Resources/Map/99_poland/spaces/99_poland/outland/height_map_cascade_0.png"
	//"../Bin/WOT_Resources/Map/99_poland/spaces/99_poland/outland/height_map_cascade_9_16.png"
	//"../Bin/Resources/Textures/Terrain/Height.bmp"


	_matrix		PreTransformMatrix = PreTransformMatrix = XMMatrixIdentity();

	/* For.Prototype_Component_Model_VHouse01A */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_VHouse01A"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Map/99_poland//content/Buildings/060_VHouse01A/hd_bld_EU_060_VHouse_01A.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_ChurchA */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_ChurchA"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Map/99_poland//content/Buildings/056_ChurchA/hd_bld_EU_056_ChurchA.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Tiger */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Tiger"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/German/G04_PzVI_Tiger_I/Tiger I.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_TigerBody */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerBody"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/German/G04_PzVI_Tiger_I/Tiger I_Body.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_TigerDestroyed */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerDestroyed"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/German/G04_PzVI_Tiger_I/Tiger I_CRASHED.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_FuryDestroyed */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryDestroyed"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/America/A104_M4A3E8A/Fury_CRASHED.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fury */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Fury"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/America/A104_M4A3E8A/Fury.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_FuryBody */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryBody"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/America/A104_M4A3E8A/Fury_Body.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_FuryTurret */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryTurret"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/America/A104_M4A3E8A/FuryTurret.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_FuryGun */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryGun"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/America/A104_M4A3E8A/FuryGun.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_BurntTree */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_BurntTree"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Map/99_poland/content/Environment/hd_env_UNI_073_BurntTree/hd_env_UNI_073_BurntTree_01.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Skydome */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Skydome"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Map/99_poland/maps/skyboxes/99_Poland_sky/skydome/99_Poland_sky_forward.bin", PreTransformMatrix))))
		return E_FAIL;
#pragma endregion

#pragma region 원형 객체
	lstrcpy(m_szLoadingText, TEXT("원형 객체을(를) 로딩중입니다."));

	/* For.Prototype_GameObject_GameManager */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_GameManager"),
		CGameManager::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DamagePanel */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamagePanel"),
		CDamagePanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Minimap */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Minimap"),
		CMinimap::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_StatusLight */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_StatusLight"),
		CStatusLight::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Engine */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Engine"),
		CEngine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BurntTree */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_BurntTree"),
		CBurntTree::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_VHouse01A */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_VHouse01A"),
		CVHouse01A::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChurchA */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_ChurchA"),
		CChurchA::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fury */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Fury"),
		CFury::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tiger */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tiger"),
		CTiger::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skydome */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Skydome"),
		CSkydome::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_TPS */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_TPS"),
		CCamera_TPS::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tool_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tool_Base"),
		CTool_Base::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

	bLoadStatic = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_BackGround*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Logo/login_bg.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Logo*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Logo/login_logo_big.dds"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Logo */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo"),
		CLogo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button_Login */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Button_Login"),
		CButton_Login::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Hanger()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_Button_Fury*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_Button_Fury"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Tanks/a104_m4a3e8a.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Button_Tiger*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_Button_Tiger"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Tanks/g04_pzvi_tiger_i.dds"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	/* For.Prototype_GameObject_Button_Start */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Start"),
		CButton_Start::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button_Fury */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Fury"),
		CButton_Fury::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fury_Hanger */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Fury_Hanger"),
		CFury_Hanger::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button_Tiger */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Tiger"),
		CButton_Tiger::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tiger_Hanger */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Tiger_Hanger"),
		CTiger_Hanger::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Practice()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Tool_EngineSound */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::PRACTICE), TEXT("Prototype_GameObject_Tool_EngineSound"),
		CTool_Engine_Sound::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice,m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice,m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	///* For.Prototype_Component_VIBuffer_Cube */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
	//	CVIBuffer_Cube::Create(m_pGraphic_Device))))
	//	return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice,m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_MapTool()
{
	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_MapTool */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_MapTool"),
		CMapTool::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MapObject */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_MapObject"),
		CMapObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	CoUninitialize();

	DeleteCriticalSection(&m_CriticalSection);
}
