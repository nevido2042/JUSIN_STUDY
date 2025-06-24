#include "Loader.h"

#include "GameInstance.h"

#include "PacketType.h"

#pragma region LODING
#include "Level_Loading.h"
#include "BackGround_Loading.h"
#include "Loading_Spinner.h"
#pragma endregion

#pragma region STATIC
#include "Smoke.h"
#include "Ash.h"
#include "Shed.h"
#include "WorkshopNewRoof.h"
#include "KitCrashFactoryWall01A.h"
#include "PolishSignboards.h"
#include "Fury_Chassis.h"
#include "Tiger_Chassis.h"
#include "AimCircle.h"
#include "InvisibleWall.h"
#include "DamageBig.h"
#include "DamageIndicator.h"
#include "Boundary.h"
#include "Score_Tank.h"
#include "Score.h"
#include "PickedManager.h"
#include "Icon_Consumables.h"
#include "CountdownTimer.h"
#include "DamageBar_World.h"
#include "FPS_Renderer.h"
#include "Shell.h"
#include "DirectionBody.h"
#include "DirectionTurret.h"
#include "GunMarker.h"
#include "DamageBar.h"
#include "Crosshair.h"
#include "Minimap_Tank.h"
#include "PersonalArrowEntry.h"
#include "Icon_Module.h"
#include "TigerTrackRight.h"
#include "TigerTrackLeft.h"
#include "TigerGun.h"
#include "TigerGunSkin.h"
#include "TigerTurret.h"
#include "TigerTurretSkin.h"
#include "FuryGun.h"
#include "FuryTrackRight.h"
#include "FuryTrackLeft.h"
#include "FuryTurret.h"
#include "GameManager.h"
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
#include "Camera_FPS.h"
#include "Terrain.h"
#include "Engine.h"
#include "AmmoBay.h"
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
#include "Tiger_Hanger.h"
#include "Button_Fury.h"
#include "Button_Tiger.h"
#include "Button_Start.h"
#include "Button_Practice.h"
#include "Button_Customize.h"
#include "Button_Exit_Customize.h"
#include "Button_Color.h"
#include "Button_Part.h"
#include "Background_UI.h"
#pragma endregion

#pragma region MAPTOOL
#include "MapTool.h"
#include "MapObject.h"
#pragma endregion

#pragma region PRACTICE
#include "DamageCollider.h"
#pragma endregion

#pragma region GAMEPLAY

#pragma endregion

#pragma region PARTICLETTOOL
#include "ParticleTool.h"
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

	case LEVEL::PARTICLETOOL:
		hr = Loading_For_ParticleTool();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}



HRESULT CLoader::Loading_For_Static()
{

#pragma region 셰이더
	/* For.Prototype_Component_Shader_VtxRectInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxRectInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxRectInstance.hlsl"), VTXRECT_PARTICLE_INSTANCE::Elements, VTXRECT_PARTICLE_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosInstance.hlsl"), VTXPOS_PARTICLE_INSTANCE::Elements, VTXPOS_PARTICLE_INSTANCE::iNumElements))))
		return E_FAIL;
#pragma endregion


#pragma region 폰트
	/* MakeSpriteFont "WarheliosKO" /FontSize:60 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 WarheliosKO.spritefont */
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_WarheliosKO"), TEXT("../Bin/WOT_Resources/Font/WarheliosKO.spritefont"))))
		return E_FAIL;
#pragma endregion

#pragma region 텍스쳐
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	/* For.Prototype_Component_Texture_Smoke*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Smoke"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/Particle/eff_tex/output256x256/misc/033.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Ash*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Ash"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/Particle/eff_tex/output256x256/misc/032.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_DamageBig*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DamageBig"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/damageIndicator/output/damageBig_ex.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Score_Tank_Red*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Score_Tank_Red"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/vehicleMarkerAtlas/output/red_mediumTank.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Score_Tank_Green*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Score_Tank_Green"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/vehicleMarkerAtlas/output/green_mediumTank.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Back_Ghost*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Back_Ghost"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/components/output/back_ghost.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_SmallRepairkit*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_SmallRepairkit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/InGame/Artefact/smallRepairkit.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_HandExtinguishers*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_HandExtinguishers"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/InGame/Artefact/handExtinguishers.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_SmallMedkit*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_SmallMedkit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/InGame/Artefact/smallMedkit.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_DamageBar_World_Red */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DamageBar_World_Red"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/vehicleMarkerAtlas/output/damageBar_red.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_DamageBar_World_Green */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DamageBar_World_Green"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/vehicleMarkerAtlas/output/damageBar_green.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_DirectionBody */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DirectionBody"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/InGame/Direction/body.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_DirectionTurret */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DirectionTurret"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/InGame/Direction/turret.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_GunMarker */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_GunMarker"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/InGame/Crosshair/swf/crosshairControls/crosshairControls-11.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_DamageBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DamageBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/battleAtlas/output/damageBar_green.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Crosshair */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Crosshair"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/InGame/Crosshair/Crosshair.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MediumTank_Enemy_Red */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_MediumTank_Enemy_Red"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/battleAtlas/output/mediumTank_enemy_red.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MediumTank_Ally_Green */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_MediumTank_Ally_Green"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/battleAtlas/output/mediumTank_ally_green.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_PersonalArrowEntry */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_PersonalArrowEntry"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/battleAtlas/PersonalArrowEntry/PersonalArrowEntry.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Button_Login */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Button_Login"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/components/output/back_primary_red.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/Map/99_poland/spaces/99_poland/global_AM.dds"), 1))))
		return E_FAIL;
	//"../Bin/WOT_Resources/Map/99_poland/maps/landscape/00_AllTerrain3_tiles/Foliage_yellow_02_00236_am.dds"
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

	/* For.Prototype_Component_Texture_Icon_Engine */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Icon_Engine"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/battleAtlas/Icon_Module/engine%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Icon_AmmoBay */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Icon_AmmoBay"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/battleAtlas/Icon_Module/ammoBay%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Icon_TurretRotator */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Icon_TurretRotator"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/battleAtlas/Icon_Module/turretRotator%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Icon_Track */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Icon_Track"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/battleAtlas/Icon_Module/chassis%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Icon_FuelTank */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Icon_FuelTank"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/battleAtlas/Icon_Module/fuelTank%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Icon_Gun */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Icon_Gun"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/battleAtlas/Icon_Module/gun%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Icon_Radio */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Icon_Radio"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/battleAtlas/Icon_Module/radio%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Icon_SurveyingDevice */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Icon_SurveyingDevice"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/battleAtlas/Icon_Module/surveyingDevice%d.dds"), 3))))
		return E_FAIL;

#pragma endregion

#pragma region 모델
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));

	/* For.Prototype_Component_VIBuffer_Snow */
	CVIBuffer_Rect_Instance::RECT_INSTANCE_DESC		AshDesc{};
	AshDesc.iNumInstance = 25000;
	AshDesc.vCenter = _float3(64.f, 30.f, 64.0f);
	AshDesc.vRange = _float3(512.f, 3.0f, 512.f);
	AshDesc.vSize = _float2(0.1f, 0.4f);
	AshDesc.vLifeTime = _float2(20.f, 30.f);
	AshDesc.vSpeed = _float2(1.f, 3.f);
	AshDesc.isLoop = true;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Ash"),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pContext, &AshDesc))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Explosion*/
	CVIBuffer_Point_Instance::POINT_INSTANCE_DESC		SmokeDesc{};
	SmokeDesc.iNumInstance = 500;
	SmokeDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	SmokeDesc.vRange = _float3(0.2f, 0.2f, 0.2f);
	SmokeDesc.vSize = _float2(0.05f, 0.1f);
	SmokeDesc.vLifeTime = _float2(0.5f, 2.f);
	SmokeDesc.vSpeed = _float2(1.f, 2.f);
	SmokeDesc.vPivot = _float3(0.0f, 0.f, 0.f);
	SmokeDesc.isLoop = true;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Smoke"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &SmokeDesc))))
		return E_FAIL;





	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/Map/99_poland/spaces/99_poland/outland/height_map_cut_128.png"), { TERRAIN_OFFSET_WIDTH, TERRAIN_OFFSET_HEIGHT}))))
		return E_FAIL;

	//height_map_cut_128
	//"../Bin/WOT_Resources/Map/99_poland/spaces/99_poland/outland/height_map_cascade_0.png"
	//"../Bin/WOT_Resources/Map/99_poland/spaces/99_poland/outland/height_map_cascade_9_16.png"
	//"../Bin/Resources/Textures/Terrain/Height.bmp"


	_matrix		PreTransformMatrix = PreTransformMatrix = XMMatrixIdentity();

	/* For.Prototype_Component_Model_Shed */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Shed"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Map/99_poland//content/Buildings/057_Shed/hd_bld_EU_057_Shed_01A.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_WorkshopNewRoof */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_WorkshopNewRoof"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Map/99_poland//content/Buildings/008_WorkshopNew/hd_bld_UNI_008_WorkshopNew_Roof_03A.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_KitCrashFactoryWall */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_KitCrashFactoryWall"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Map/99_poland//content/Buildings/006_KitCrashFactoryA/hd_bld_UNI_006_KitCrashFactory_Wall_01A.bin", PreTransformMatrix))))
		return E_FAIL;

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

#pragma region Tiger
	/* For.Prototype_Component_Model_Tiger */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Tiger"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/German/G04_PzVI_Tiger_I/Tiger I.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_TigerTrackRight */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerTrackRight"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/German/G04_PzVI_Tiger_I/Tiger I_TrackRight.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_TigerTrackLeft */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerTrackLeft"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/German/G04_PzVI_Tiger_I/Tiger I_TrackLeft.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_TigerGun */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerGun"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/German/G04_PzVI_Tiger_I/Tiger I_Gun.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_TigerGunSkin */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerGunSkin"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/German/G04_PzVI_Tiger_I/Tiger I_Skin_Gun.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_TigerTurret */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerTurret"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/German/G04_PzVI_Tiger_I/Tiger I_Turret.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_TigerTurretSkin */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerTurretSkin"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/German/G04_PzVI_Tiger_I/Tiger I_Skin_Turret.bin", PreTransformMatrix))))
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
#pragma endregion

#pragma region Fury
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

	/* For.Prototype_Component_Model_FuryTrackLeft */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryTrackLeft"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/America/A104_M4A3E8A/Fury_TrackLeft.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_FuryTrackRight */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryTrackRight"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/America/A104_M4A3E8A/Fury_TrackRight.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_FuryGun */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryGun"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Tank/America/A104_M4A3E8A/FuryGun.bin", PreTransformMatrix))))
		return E_FAIL;
#pragma endregion

	/* For.Prototype_Component_Model_BurntTree */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_BurntTree"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Map/99_poland/content/Environment/hd_env_UNI_073_BurntTree/hd_env_UNI_073_BurntTree_01.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_PolishSignboards */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_PolishSignboards"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/WOT_Resources/Map/99_poland/content/Environment/hd_env_EU_064_PolishSignboards/hd_env_EU_064_PolishSignboards_01.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Skydome */
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(90.f));
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

	/* For.Prototype_GameObject_Ash */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Smoke"),
		CSmoke::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ash */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Ash"),
		CAsh::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fury_Chassis */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Fury_Chassis"),
		CFury_Chassis::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tiger_Chassis */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tiger_Chassis"),
		CTiger_Chassis::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_AimCircle */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_AimCircle"),
		CAimCircle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_InvisibleWall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_InvisibleWall"),
		CInvisibleWall::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DamageIndicator */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageIndicator"),
		CDamageIndicator::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Boundary */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Boundary"),
		CBoundary::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Score_Tank */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Score_Tank"),
		CScore_Tank::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Score */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Score"),
		CScore::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_PickedManager */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_PickedManager"),
		CPickedManager::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Icon_Consumables */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Icon_Consumables"),
		CIcon_Consumables::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DamageBar_World */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageBar_World"),
		CDamageBar_World::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_CountdownTimer */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_CountdownTimer"),
		CCountdownTimer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FPS_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FPS_Renderer"),
		CFPS_Renderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Shell */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Shell"),
		CShell::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DirectionBody */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DirectionBody"),
		CDirectionBody::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DirectionTurret */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DirectionTurret"),
		CDirectionTurret::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_GunMarker */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_GunMarker"),
		CGunMarker::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DamageBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageBar"),
		CDamageBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Crosshair */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Crosshair"),
		CCrosshair::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_PersonalArrowEntry */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_PersonalArrowEntry"),
		CPersonalArrowEntry::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Minimap_Tank */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Minimap_Tank"),
		CMinimap_Tank::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Icon_Module */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Icon_Module"),
		CIcon_Module::Create(m_pDevice, m_pContext))))
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

	/* For.Prototype_GameObject_AmmoBay */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_AmmoBay"),
		CAmmoBay::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma region 맵 오브젝트
	/* For.Prototype_GameObject_BurntTree */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_BurntTree"),
		CBurntTree::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_PolishSignboards */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_PolishSignboards"),
		CPolishSignboards::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Shed */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Shed"),
		CShed::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_WorkshopNewRoof */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_WorkshopNewRoof"),
		CWorkshopNewRoof::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_KitCrashFactoryWall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_KitCrashFactoryWall"),
		CKitCrashFactoryWall01A::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_VHouse01A */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_VHouse01A"),
		CVHouse01A::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChurchA */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_ChurchA"),
		CChurchA::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion


#pragma region Fury
	/* For.Prototype_GameObject_Fury */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Fury"),
		CFury::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FuryTurret */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryTurret"),
		CFuryTurret::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FuryGun */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryGun"),
		CFuryGun::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FuryTrackLeft */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryTrackLeft"),
		CFuryTrackLeft::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FuryTrackRight */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryTrackRight"),
		CFuryTrackRight::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Tiger
	/* For.Prototype_GameObject_Tiger */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tiger"),
		CTiger::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TigerTrackLeft */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerTrackLeft"),
		CTigerTrackLeft::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TigerTrackRight */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerTrackRight"),
		CTigerTrackRight::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TigerTurret */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerTurret"),
		CTigerTurret::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TigerTurretSkin */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerTurretSkin"),
		CTigerTurretSkin::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TigerGun */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerGun"),
		CTigerGun::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TigerGunSkin */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerGunSkin"),
		CTigerGunSkin::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

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

	/* For.Prototype_GameObject_Camera_FPS */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_FPS"),
		CCamera_FPS::Create(m_pDevice, m_pContext))))
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
#pragma region 텍스쳐
	
#pragma region 커스터마이징
	/* For.Prototype_Component_Texture_CustomColor_Default*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_CustomColor_Default"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/components/output/closeBtnIcon.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CustomColor_Yellow*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_CustomColor_Yellow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Customize/Repaints/custom_color_37.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CustomColor_Red*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_CustomColor_Red"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Customize/Repaints/custom_color_18.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CustomColor_Blue*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_CustomColor_Blue"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Customize/Repaints/custom_color_22.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CustomColor_Black*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_CustomColor_Black"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Customize/Repaints/custom_color_23.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CustomColor_White*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_CustomColor_White"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Customize/Repaints/custom_color_29.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CustomColor_Brown*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_CustomColor_Brown"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Customize/Repaints/uk_color_02.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CustomColor_Green*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_CustomColor_Green"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Customize/Repaints/ussr_color_07.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CustomColor_Part_Turret*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_CustomColor_Part_Turret"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Customize/Parts/tower.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CustomColor_Part_Gun*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_CustomColor_Part_Gun"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Customize/Parts/gun.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CustomColor_Part_Body*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_CustomColor_Part_Body"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Customize/Parts/hull.dds"), 1))))
		return E_FAIL;
#pragma endregion

	/* For.Prototype_Component_Texture_Background_UI*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_Background_UI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Atlas/components/output/cm_sub_item_bg.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Button_Fury*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_Button_Fury"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Tanks/a104_m4a3e8a.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Button_Tiger*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_Button_Tiger"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Tanks/g04_pzvi_tiger_i.dds"), 1))))
		return E_FAIL;
#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

#pragma region 원형 객체
	/* For.Prototype_GameObject_Button_Color */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Color"),
		CButton_Color::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button_Part */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Part"),
		CButton_Part::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Background_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Background_UI"),
		CBackground_UI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button_Exit_Customize */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Exit_Customize"),
		CButton_Exit_Customize::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button_Customize */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Customize"),
		CButton_Customize::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button_Practice */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Practice"),
		CButton_Practice::Create(m_pDevice, m_pContext))))
		return E_FAIL;

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
#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Practice()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	/* For.Prototype_GameObject_DamageCollider */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::PRACTICE), TEXT("Prototype_GameObject_DamageCollider"),
		CDamageCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	//Send Wait Game

	/*PACKET_DESC Desc;
	Desc.iID = m_pGameInstance->Get_ID();
	m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_JOIN_MATCH), &Desc);*/

	CGameManager* pGameManager = static_cast<CGameManager*>(m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Layer_GameManager")));

	while (true)
	{
		if (pGameManager->Get_isGameStart())
		{
			cout << "Game_Start" << endl;
			break;
		}
			
	}

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

HRESULT CLoader::Loading_For_ParticleTool()
{
	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_ParticleTool */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::PARTICLETOOL), TEXT("Prototype_GameObject_ParticleTool"),
		CParticleTool::Create(m_pDevice, m_pContext))))
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
