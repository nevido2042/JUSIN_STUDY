#include "Level_Hanger.h"

#include "GameInstance.h"
#include "Level_Loading.h"

//#include "Camera_Free.h"
#include "Camera_Hanger.h"
#include "Tank.h"

#include "UIObject.h"
#include "GameManager.h"
#include "LandObject.h"
#include "Button_Color.h"
#include "Button_Part.h"
#include "Button_3DCustom.h"

CLevel_Hanger::CLevel_Hanger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Hanger::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	//if (FAILED(Ready_Layer_GameManger(TEXT("Layer_GameManager"))))
	//	return E_FAIL;

	static_cast<CGameManager*>(m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Layer_GameManager")))->Set_Select_Tank(TANK::FURY);

	if (FAILED(Ready_Layer_StatusLight(TEXT("Layer_StatusLight"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_FPS_Renderer(TEXT("Layer_FPS_Renderer"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Load_Map()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Background_UI(TEXT("Layer_Background_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_TopBar(TEXT("Layer_TopBar"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MapVegetation(TEXT("Layer_MapVegetation"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Planes(TEXT("Layer_Plane"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Ash(TEXT("Layer_Ash"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Explosion(TEXT("Layer_Explosion"))))
	//	return E_FAIL;

#pragma region 기본 버튼
	if (FAILED(Ready_Layer_Button_Start(TEXT("Layer_Button"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button_Practice(TEXT("Layer_Button"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button_Customize(TEXT("Layer_Button"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button_Fury(TEXT("Layer_Button"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button_Tiger(TEXT("Layer_Button"))))
		return E_FAIL;
#pragma endregion



#pragma region 커스터마이징용 버튼
	if (FAILED(Ready_Layer_Button_Exit_Customize(TEXT("Layer_Button_Customize"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button_Colors(TEXT("Layer_Button_Customize"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button_Parts(TEXT("Layer_Button_Customize"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button_3DCustoms(TEXT("Layer_Button_3DCustom"))))
		return E_FAIL;

#pragma endregion

	if (FAILED(Ready_Layer_Fury_Hanger(TEXT("Layer_Fury_Hanger"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Tiger_Hanger(TEXT("Layer_Tiger_Hanger"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Skydome(TEXT("Layer_Skydome"))))
		return E_FAIL;


	//if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Camera_Hanger(TEXT("Layer_Camera"))))
		return E_FAIL;

	CGameManager* pGameManager = GET_GAMEMANAGER;
	if (pGameManager)
	{
		//게임 중이었다면, 종료했음을 저장하고 점수판을 생성해라
		if (pGameManager->Get_isGameStart() == true)
		{
			pGameManager->Set_isGameStart(false);

			if (FAILED(Ready_Layer_ScoreBoard(TEXT("Layer_ScoreBoard"))))
				return E_FAIL;
		}
	}

	pGameManager->StopBGM_Game();
	pGameManager->StopBGM();

	pGameManager->PlayBGM();
	//if (FAILED(Ready_Layer_ScoreBoard(TEXT("Layer_ScoreBoard"))))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_Hanger::Update(_float fTimeDelta)
{
	if (m_bLevelChanged)
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, static_cast<LEVEL>(m_iNewLevelIndex)))))
			return;
	}
	else if (m_pGameInstance->Key_Down(DIK_P))
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::PARTICLETOOL))))
			return;
	}
	else if (m_pGameInstance->Key_Down(DIK_T))
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::MAPTOOL))))
			return;
	}
}

HRESULT CLevel_Hanger::Render()
{
	SetWindowText(g_hWnd, TEXT("격납고 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Lights()
{
	CShadow::SHADOW_DESC		Desc{};
	Desc.vEye = _float4(TERRAIN_OFFSET_WIDTH * TERRAIN_SIZE * 0.75f, 150.f, TERRAIN_OFFSET_WIDTH * TERRAIN_SIZE * 0.75f, 1.f);
	Desc.vAt = _float4(TERRAIN_OFFSET_WIDTH * TERRAIN_SIZE * 0.5f, 0.f, TERRAIN_OFFSET_WIDTH * TERRAIN_SIZE * 0.25f, 1.f);
	Desc.fFovy = XMConvertToRadians(40.0f);
	Desc.fNear = 0.1f;
	Desc.fFar = CAMERA_FAR;

	if (FAILED(m_pGameInstance->Ready_Light_For_Shadow(Desc)))
		return E_FAIL;

	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	XMStoreFloat4(&LightDesc.vDirection, XMVector4Normalize(XMLoadFloat4(&Desc.vAt) - XMLoadFloat4(&Desc.vEye))); //_float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.fAmbient = 1.f;
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

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
	//CCamera_Free::CAMERA_FREE_DESC Desc = {};

	//Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	//Desc.fSpeedPerSec = 10.0f;
	//lstrcpy(Desc.szName, TEXT("Camera_Free"));

	//Desc.vEye = _float3(325.f, 90.f, 291.f);
	//Desc.vAt = _float3(60.f, 80.f, 0.f);
	//Desc.fFov = XMConvertToRadians(60.0f);
	//Desc.fNear = 0.1f;
	//Desc.fFar = 500.f;

	//Desc.fSensor = 0.1f;

	////Desc.vInitPosition = { 1000.f, 1000.f, 1000.f };

	//if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
	//	ENUM_CLASS(LEVEL::HANGER), strLayerTag, &Desc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Camera_Hanger(const _wstring strLayerTag)
{
	CCamera_Hanger::CAMERA_HANGER_DESC Desc{};
	Desc.pTarget = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Layer_Fury_Hanger"));
	Desc.bActive = true;
	lstrcpy(Desc.szName, TEXT("Camera_TPS"));

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Camera_Hanger"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Background_UI(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = static_cast<_float>(g_iWinSizeX);
	UIObject_Desc.fSizeY = 150.f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY * 0.8f;


	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Background_UI"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_TopBar(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = static_cast<_float>(g_iWinSizeX);
	UIObject_Desc.fSizeY = 100.f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY * 0.04f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_TopBar"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_ScoreBoard(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 500.f* UI_RATIO;
	UIObject_Desc.fSizeY = 500.f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX * 0.2f;
	UIObject_Desc.fY = g_iWinSizeY * 0.5f;
	
	UIObject_Desc.fDepth = DEPTH_BACKGROUND;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_ScoreBoard"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Button_Start(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 300.f * UI_RATIO;
	UIObject_Desc.fSizeY = 80.f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX * 0.5f;
	UIObject_Desc.fY = g_iWinSizeY * 0.04f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND - 0.01f;


	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Start"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Button_Practice(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 300.f * UI_RATIO;
	UIObject_Desc.fSizeY = 80.f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX * 0.3f;
	UIObject_Desc.fY = g_iWinSizeY * 0.04f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND - 0.01f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Practice"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Button_Customize(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 150.f * UI_RATIO;
	UIObject_Desc.fSizeY = 40.f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX * 0.9f;
	UIObject_Desc.fY = g_iWinSizeY * 0.67f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND - 0.01f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Customize"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Button_Exit_Customize(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 150.f * UI_RATIO;
	UIObject_Desc.fSizeY = 40.f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX * 0.9f;
	UIObject_Desc.fY = g_iWinSizeY * 0.7f;
	UIObject_Desc.bVisible = false;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND - 0.01f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Exit_Customize"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Button_Colors(const _wstring strLayerTag)
{
	CButton_Color::BUTTON_COLOR_DESC				ButtonColorDesc{};

	ButtonColorDesc.fSizeX = 102.f * UI_RATIO;
	ButtonColorDesc.fSizeY = 102.f * UI_RATIO;
	ButtonColorDesc.fY = g_iWinSizeY * 0.8f;
	ButtonColorDesc.bVisible = false;

	ButtonColorDesc.fX = g_iWinSizeX * 0.5f;
	ButtonColorDesc.vBaseColor = _float4(2.f, 2.f, 1.f, 1.f);
	ButtonColorDesc.strTextureName = TEXT("Prototype_Component_Texture_CustomColor_Yellow");
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Color"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &ButtonColorDesc)))
		return E_FAIL;

	ButtonColorDesc.fX = g_iWinSizeX * 0.45f;
	ButtonColorDesc.vBaseColor = _float4(2.f, 2.f, 2.f, 1.f);
	ButtonColorDesc.strTextureName = TEXT("Prototype_Component_Texture_CustomColor_White");
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Color"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &ButtonColorDesc)))
		return E_FAIL;

	ButtonColorDesc.fX = g_iWinSizeX * 0.40f;
	ButtonColorDesc.vBaseColor = _float4(0.2f, 0.2f, 0.2f, 1.f);
	ButtonColorDesc.strTextureName = TEXT("Prototype_Component_Texture_CustomColor_Black");
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Color"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &ButtonColorDesc)))
		return E_FAIL;

	ButtonColorDesc.fX = g_iWinSizeX * 0.35f;
	ButtonColorDesc.vBaseColor = _float4(1.f, 1.f, 2.f, 1.f);
	ButtonColorDesc.strTextureName = TEXT("Prototype_Component_Texture_CustomColor_Blue");
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Color"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &ButtonColorDesc)))
		return E_FAIL;

	ButtonColorDesc.fX = g_iWinSizeX * 0.30f;
	ButtonColorDesc.vBaseColor = _float4(2.f, 1.f, 1.f, 1.f);
	ButtonColorDesc.strTextureName = TEXT("Prototype_Component_Texture_CustomColor_Red");
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Color"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &ButtonColorDesc)))
		return E_FAIL;

	ButtonColorDesc.fX = g_iWinSizeX * 0.25f;
	ButtonColorDesc.vBaseColor = _float4(0.42f, 1.09f, 0.65f, 1.f);
	ButtonColorDesc.strTextureName = TEXT("Prototype_Component_Texture_CustomColor_Green");
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Color"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &ButtonColorDesc)))
		return E_FAIL;

	ButtonColorDesc.fX = g_iWinSizeX * 0.20f;
	ButtonColorDesc.vBaseColor = _float4(1.09f, 0.90f, 0.73f, 1.f);
	ButtonColorDesc.strTextureName = TEXT("Prototype_Component_Texture_CustomColor_Brown");
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Color"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &ButtonColorDesc)))
		return E_FAIL;

	ButtonColorDesc.fX = g_iWinSizeX * 0.15f;
	ButtonColorDesc.vBaseColor = _float4(1.f, 1.f, 1.f, 1.f);
	ButtonColorDesc.strTextureName = TEXT("Prototype_Component_Texture_CustomColor_Default");
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Color"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &ButtonColorDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Button_Parts(const _wstring strLayerTag)
{
	CButton_Part::BUTTON_PART_DESC				ButtonPartDesc{};

	ButtonPartDesc.fSizeX = 59.f * 2.f * UI_RATIO;
	ButtonPartDesc.fSizeY = 20.f * 2.f * UI_RATIO;
	ButtonPartDesc.fX = g_iWinSizeX * 0.05f;
	ButtonPartDesc.bVisible = false;

	ButtonPartDesc.fY = g_iWinSizeY * 0.8f;
	ButtonPartDesc.strTextureName = TEXT("Prototype_Component_Texture_CustomColor_Part_Body");
	ButtonPartDesc.ePartRepaint = PART_REPAINT::BODY;
	lstrcpy(ButtonPartDesc.szName, TEXT("Button_Repaint_Body"));

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Part"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &ButtonPartDesc)))
		return E_FAIL;

	ButtonPartDesc.fY = g_iWinSizeY * 0.75f;
	ButtonPartDesc.strTextureName = TEXT("Prototype_Component_Texture_CustomColor_Part_Turret");
	ButtonPartDesc.ePartRepaint = PART_REPAINT::TURRET;
	lstrcpy(ButtonPartDesc.szName, TEXT("Button_Repaint_Turret"));

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Part"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &ButtonPartDesc)))
		return E_FAIL;

	ButtonPartDesc.fY = g_iWinSizeY * 0.85f;
	ButtonPartDesc.strTextureName = TEXT("Prototype_Component_Texture_CustomColor_Part_Gun");
	ButtonPartDesc.ePartRepaint = PART_REPAINT::GUN;
	lstrcpy(ButtonPartDesc.szName, TEXT("Button_Repaint_Gun"));

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Part"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &ButtonPartDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Button_3DCustoms(const _wstring strLayerTag)
{
	CButton_3DCustom::BUTTON_3DCUSTOM_DESC				Desc{};

	Desc.fSizeX = 102.f * UI_RATIO;
	Desc.fSizeY = 102.f * UI_RATIO;
	Desc.fY = g_iWinSizeY * 0.8f;
	Desc.bVisible = false;

	Desc.fX = g_iWinSizeX * 0.55f;
	Desc.strTextureName = TEXT("Prototype_Component_Texture_3DCustom_Monkey");
	Desc.e3DCustom = CUSTOM3D::MONKEY;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_3DCustom"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &Desc)))
		return E_FAIL;

	Desc.fX = g_iWinSizeX * 0.60f;
	Desc.strTextureName = TEXT("Prototype_Component_Texture_3DCustom_DragonHead");
	Desc.e3DCustom = CUSTOM3D::DRAGONHEAD;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_3DCustom"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &Desc)))
		return E_FAIL;

	Desc.fX = g_iWinSizeX * 0.65f;
	Desc.strTextureName = TEXT("Prototype_Component_Texture_3DCustom_Drum");
	Desc.e3DCustom = CUSTOM3D::DRUM;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_3DCustom"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &Desc)))
		return E_FAIL;

	Desc.fX = g_iWinSizeX * 0.70f;
	Desc.strTextureName = TEXT("Prototype_Component_Texture_3DCustom_Speaker");
	Desc.e3DCustom = CUSTOM3D::SPEAKER;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_3DCustom"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &Desc)))
		return E_FAIL;

	Desc.fX = g_iWinSizeX * 0.75f;
	Desc.strTextureName = TEXT("Prototype_Component_Texture_3DCustom_Guitar");
	Desc.e3DCustom = CUSTOM3D::GUITAR;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_3DCustom"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &Desc)))
		return E_FAIL;

	Desc.fX = g_iWinSizeX * 0.80f;
	Desc.strTextureName = TEXT("Prototype_Component_Texture_3DCustom_Shark");
	Desc.e3DCustom = CUSTOM3D::SHARK;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_3DCustom"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &Desc)))
		return E_FAIL;

	Desc.fX = g_iWinSizeX * 0.85f;
	Desc.strTextureName = TEXT("Prototype_Component_Texture_3DCustom_GoldPot");
	Desc.e3DCustom = CUSTOM3D::GOLDPOT;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_3DCustom"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &Desc)))
		return E_FAIL;

	Desc.fX = g_iWinSizeX * 0.90f;
	Desc.strTextureName = TEXT("Prototype_Component_Texture_CustomColor_Default");
	Desc.e3DCustom = CUSTOM3D::END;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_3DCustom"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Button_Fury(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 420.f * UI_RATIO;
	UIObject_Desc.fSizeY = 307.f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX * 0.4f;
	UIObject_Desc.fY = g_iWinSizeY * 0.8f;


	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Fury"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Button_Tiger(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 420.f * UI_RATIO;
	UIObject_Desc.fSizeY = 307.f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX * 0.6f;
	UIObject_Desc.fY = g_iWinSizeY * 0.8f;


	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Tiger"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Fury_Hanger(const _wstring strLayerTag)
{
	CTank::TANK_DESC Desc = {};
	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("Fury_Hanger"));
	Desc.vInitPosition = _float3(330.f, 0.f, 235.f);
	Desc.iID = -100;
	Desc.vBodyColor = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Fury"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &Desc)))
		return E_FAIL;

	//CGameObject::GAMEOBJECT_DESC DescPlane = {};
	//DescPlane.vInitPosition = _float3(330.f, 90.f, 235.f);
	//if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Plane"),
	//	ENUM_CLASS(LEVEL::HANGER), TEXT("Layer_Plane"), &DescPlane)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Tiger_Hanger(const _wstring strLayerTag)
{
	CTank::TANK_DESC Desc = {};
	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("Tiger_Hanger"));
	Desc.vInitPosition = _float3(330.f, 0.f, 235.f);
	Desc.bActive = false;
	Desc.iID = -100;
	Desc.vBodyColor = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tiger"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Skydome(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Skydome"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Ash(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Ash"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Explosion(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Explosion"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_MapVegetation(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_MapVegetation"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_Planes(const _wstring strLayerTag)
{
	for (_uint i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Plane"),
			ENUM_CLASS(LEVEL::HANGER), TEXT("Layer_Plane"))))
			return E_FAIL;
	}

	return S_OK;
}


HRESULT CLevel_Hanger::Ready_Layer_StatusLight(const _wstring strLayerTag)
{
	//이미 있으면 안만듬
	if (m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::STATIC), TEXT("Layer_StatusLight")))
		return S_OK;

	CUIObject::UIOBJECT_DESC				UIObject_Desc{};

	UIObject_Desc.fSizeX = 40.0f * UI_RATIO;
	UIObject_Desc.fSizeY = 50.0f * UI_RATIO;
	UIObject_Desc.fX = g_iWinSizeX * 0.05f;
	UIObject_Desc.fY = g_iWinSizeY * 0.015f;


	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_StatusLight"),
		ENUM_CLASS(LEVEL::STATIC), strLayerTag, &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_GameManger(const _wstring strLayerTag)
{
	//이미 있으면 안만듬
	if (m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::STATIC), TEXT("Layer_GameManager")))
		return S_OK;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_GameManager"),
		ENUM_CLASS(LEVEL::STATIC), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hanger::Ready_Layer_FPS_Renderer(const _wstring strLayerTag)
{

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FPS_Renderer"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag)))
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
		m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), PrototypeName, ENUM_CLASS(LEVEL::HANGER), LayerName);
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
