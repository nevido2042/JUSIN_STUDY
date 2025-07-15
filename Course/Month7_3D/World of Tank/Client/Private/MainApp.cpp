#include "MainApp.h"

#include "GameInstance.h"
#include "DebugUtils.h"

#include "Level_Loading.h"

#include "BackGround_Loading.h"
#include "Loading_Spinner.h"
#include "WOT_Icon.h"

#pragma region For_Packet
#include "GameManager.h"
#include "Layer.h"
#include "Turret.h"
#include "Gun.h"
#include "Engine.h"
#include "LandObject.h"
#include "Tank.h"
#include "VIBuffer_Terrain.h"
#include "DamageIndicator.h"
#pragma endregion


CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);

}

HRESULT CMainApp::Initialize()
{

#if defined(_DEBUG)
	AllocConsole();
	SetConsoleTitleW(L"Debug Console");

	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);

	SetConsoleOutputCP(CP_UTF8);
	ios::sync_with_stdio();
#endif

	ENGINE_DESC			EngineDesc{};

	EngineDesc.hInst = g_hInst;
	EngineDesc.hWnd = g_hWnd;
#pragma message ("윈사이즈 엔진에 던져주는거 필요 없겠는데? 어차피 계속 바뀌니깐")
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.isWindowed = true;
	EngineDesc.iNumLevels = static_cast<_uint>(LEVEL::END);
	EngineDesc.iNumColliderGroup = static_cast<_uint>(COLLISION_GROUP::END);

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	Ready_Prototype_Component();
	Ready_Prototype_For_Loading();

	if (FAILED(Start_Level(LEVEL::LOGO)))
		return E_FAIL;

	if(FAILED(Ready_Packets()))
		return E_FAIL;

	//int* p = new int[4];

    return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_Network_Status() != NETWORK_STATUS::DISCONNECTED)
	{
		m_fPingElapsed += fTimeDelta;
		if (m_fPingElapsed > PING_TIME && m_bSendPing == false)
		{
			m_fPingElapsed = 0.f;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_PING), nullptr);
			m_bSendPing = true;
		}
	}

	m_pGameInstance->Update_Engine(fTimeDelta, g_iWinSizeX, g_iWinSizeY);
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Begin_Draw();

	m_pGameInstance->Draw();

	//m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), TEXT("배가 고파요"), _float2(0.f, 0.f)/*, XMVectorSet(1.f, 0.f, 0.f, 1.f)*/);

	m_pGameInstance->End_Draw();

    return S_OK;
}

HRESULT CMainApp::Ready_Packets()
{
#pragma region CONNECT
	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_GIVE_ID), [this](void* pArg)
		{
			PACKET_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(PACKET_DESC));
			m_pGameInstance->Set_ID(Desc.iID);

			cout << "My ID:" << Desc.iID << endl;

			m_pGameInstance->Set_Network_Status(NETWORK_STATUS::CONNECTED);
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_PING), [this](void* pArg)
		{
			if (m_pGameInstance->Get_Network_Status() == NETWORK_STATUS::DISCONNECTED)
				return;

			PACKET_DESC Desc{};
			Desc.iID = m_pGameInstance->Get_ID();

			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_PING);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&Desc), sizeof(PACKET_DESC));

			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_PING), [this](void* pArg)
		{
			m_pGameInstance->Set_Network_Status(NETWORK_STATUS::CONNECTED);
			//::cout << "SC_PING" << endl;
			m_bSendPing = false;
		})))
		return E_FAIL;
#pragma endregion

#pragma region READY_GAME
	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_JOIN_MATCH), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();
			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_JOIN_MATCH);
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(JOIN_MATCH_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_START_GAME), [this](void* pArg)
		{
			GAMESTART_DESC Desc = {};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(GAMESTART_DESC));
			m_pGameInstance->Clear_Packet();

			CGameManager* pGameManager = GET_GAMEMANAGER;
			pGameManager->Set_GameEndTime(Desc.GameEndTime);
			pGameManager->Set_isGameStart(true);
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_END_GAME), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();
			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_END_GAME);
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(PACKET_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_RETURN_HANGER), [this](void* pArg)
		{
			if (m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::HANGER))
			{
				m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::HANGER));
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_LOAD_COMPLETE), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();
			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_LOAD_COMPLETE);
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			PACKET_DESC* Desc = static_cast<PACKET_DESC*>(pArg);
			Desc->iID = m_pGameInstance->Get_ID();
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(Desc), sizeof(PACKET_DESC));
			m_pGameInstance->Update_Header();
			cout << "CS_LOAD_COMPLETE" << endl;
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_CREATE_MY_CHARACTER), [this](void* pArg)
		{
			SPAWN_TANK_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(SPAWN_TANK_DESC));
			m_pGameInstance->Clear_Packet();

			cout << "SC_CREATE_MY_CHARACTER" << endl;
			cout << "Pos: " << Desc.vPos.x << endl;

			CGameManager* pGameManager = GET_GAMEMANAGER;
			pGameManager->Create_My_Tank(&Desc);
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_CREATE_OTHER_CHARACTER), [this](void* pArg)
		{
			CREATE_OTHER_TANK_DESC Packet_Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Packet_Desc), sizeof(CREATE_OTHER_TANK_DESC));
			m_pGameInstance->Clear_Packet();

			CTank::TANK_DESC Desc = {};
			Desc.fRotationPerSec = 1.f;
			Desc.fSpeedPerSec = 1.f;
			Desc.iID = Packet_Desc.iID;
			Desc.vInitPosition = Packet_Desc.vPos;
			Desc.eTeam = Packet_Desc.eTeam;

			Desc.vBodyColor = Packet_Desc.vBodyColor;
			Desc.vTurretColor = Packet_Desc.vTurretColor;
			Desc.vGunColor = Packet_Desc.vGunColor;
			Desc.e3DCustom = Packet_Desc.e3DCustom;

			cout << "SC_CREATE_OTHER_CHARACTER" << endl;
			cout << "ID: " << Packet_Desc.iID << endl;

			if (Packet_Desc.eTank == TANK::FURY)
				m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Fury"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"), &Desc);
			else if (Packet_Desc.eTank == TANK::TIGER)
				m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tiger"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"), &Desc);

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_CREATE_MINIMAP), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			cout << "SC_CREATE_MINIMAP" << endl;

			CUIObject::UIOBJECT_DESC				UIObject_Desc{};

			UIObject_Desc.fSizeX = 256.0f * 1.5f * UI_RATIO;
			UIObject_Desc.fSizeY = 256.0f * 1.5f * UI_RATIO;
			UIObject_Desc.fX = g_iWinSizeX - UIObject_Desc.fSizeX * 0.5f;
			UIObject_Desc.fY = g_iWinSizeY - UIObject_Desc.fSizeY * 0.5f;

			m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Minimap"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Minimap"), &UIObject_Desc);


			CUIObject::UIOBJECT_DESC		Score_Desc{};

			Score_Desc.fSizeX = 50.0f * UI_RATIO;
			Score_Desc.fSizeY = 50.0f * UI_RATIO;
			Score_Desc.fX = g_iWinSizeX * 0.5f;
			Score_Desc.fY = g_iWinSizeY * 0.03f;
			Score_Desc.fDepth = DEPTH_BACKGROUND - 0.01f;

			if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Score"),
				ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Score"), &Score_Desc)))
				return E_FAIL;

			return S_OK;
		})))
		return E_FAIL;
#pragma endregion
	
#pragma region TANK_CONTROL
	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_PRESS_W), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_PRESS_W);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_PRESS_W), [this](void* pArg)
		{
			BOOL_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
			m_pGameInstance->Clear_Packet();

			CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
			if (pLayer == nullptr)
				return;

			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				if (Desc.iID == pGameObject->Get_ID())
				{
					//cout << "ID:" << Desc.iID << " Press W " << Desc.bBool << endl;
					static_cast<CEngine*>(pGameObject->Find_PartObject(TEXT("Part_Engine")))->Set_PressW(Desc.bBool);
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_PRESS_S), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_PRESS_S);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_PRESS_S), [this](void* pArg)
		{
			BOOL_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
			m_pGameInstance->Clear_Packet();

			CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
			if (pLayer == nullptr)
				return;

			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				if (Desc.iID == pGameObject->Get_ID())
				{
					//cout << "ID:" << Desc.iID << " Press S " << Desc.bBool << endl;
					static_cast<CEngine*>(pGameObject->Find_PartObject(TEXT("Part_Engine")))->Set_PressS(Desc.bBool);
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_PRESS_A), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_PRESS_A);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_PRESS_A), [this](void* pArg)
		{
			BOOL_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
			m_pGameInstance->Clear_Packet();

			CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
			if (pLayer == nullptr)
				return;

			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				if (Desc.iID == pGameObject->Get_ID())
				{
					//cout << "ID:" << Desc.iID << " Press A " << Desc.bBool << endl;
					static_cast<CEngine*>(pGameObject->Find_PartObject(TEXT("Part_Engine")))->Set_PressA(Desc.bBool);
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_PRESS_D), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_PRESS_D);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_PRESS_D), [this](void* pArg)
		{
			BOOL_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
			m_pGameInstance->Clear_Packet();

			CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
			if (pLayer == nullptr)
				return;

			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				if (Desc.iID == pGameObject->Get_ID())
				{
					//cout << "ID:" << Desc.iID << " Press D " << Desc.bBool << endl;
					static_cast<CEngine*>(pGameObject->Find_PartObject(TEXT("Part_Engine")))->Set_PressD(Desc.bBool);
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_LEFT), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_LEFT);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_LEFT), [this](void* pArg)
		{
			BOOL_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
			m_pGameInstance->Clear_Packet();

			CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
			if (pLayer == nullptr)
				return;

			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				if (Desc.iID == pGameObject->Get_ID())
				{
					//cout << "ID:" << Desc.iID << " Left " << Desc.bBool << endl;
					CTurret* pTurret = static_cast<CTurret*>(pGameObject->Find_PartObject(TEXT("Part_Turret")));
					pTurret->Set_Left(Desc.bBool);
					pTurret->Set_Right(false);
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_RIGHT), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_RIGHT);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_RIGHT), [this](void* pArg)
		{
			BOOL_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
			m_pGameInstance->Clear_Packet();

			CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
			if (pLayer == nullptr)
				return;

			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				if (Desc.iID == pGameObject->Get_ID())
				{
					//cout << "ID:" << Desc.iID << " Right" << Desc.bBool << endl;
					CTurret* pTurret = static_cast<CTurret*>(pGameObject->Find_PartObject(TEXT("Part_Turret")));
					pTurret->Set_Right(Desc.bBool);
					pTurret->Set_Left(false);
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_UP), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_UP);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_UP), [this](void* pArg)
		{
			BOOL_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
			m_pGameInstance->Clear_Packet();

			CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
			if (pLayer == nullptr)
				return;

			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				if (Desc.iID == pGameObject->Get_ID())
				{
					//cout << "ID:" << Desc.iID << " UP" << Desc.bBool << endl;
					CGun* pGun = static_cast<CGun*>(pGameObject->Find_PartObject(TEXT("Part_Turret"))->Find_PartObject(TEXT("Part_Gun")));
					pGun->Set_Up(Desc.bBool);
					pGun->Set_Down(false);
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_DOWN), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_DOWN);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_DOWN), [this](void* pArg)
		{
			BOOL_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
			m_pGameInstance->Clear_Packet();

			CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
			if (pLayer == nullptr)
				return;

			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				if (Desc.iID == pGameObject->Get_ID())
				{
					//cout << "ID:" << Desc.iID << " DOWN" << Desc.bBool << endl;
					CGun* pGun = static_cast<CGun*>(pGameObject->Find_PartObject(TEXT("Part_Turret"))->Find_PartObject(TEXT("Part_Gun")));
					pGun->Set_Down(Desc.bBool);
					pGun->Set_Up(false);
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_FIRE), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_FIRE);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(PACKET_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_FIRE), [this](void* pArg)
		{
			PACKET_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(PACKET_DESC));
			m_pGameInstance->Clear_Packet();

			CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
			if (pLayer == nullptr)
				return;

			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				if (Desc.iID == pGameObject->Get_ID())
				{
					//cout << "ID:" << Desc.iID << " Fire" << endl;
					static_cast<CTank*>(pGameObject)->Try_Fire();
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_DIG), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_DIG);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(POSITION_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_DIG), [this](void* pArg)
		{
			POSITION_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(POSITION_DESC));
			m_pGameInstance->Clear_Packet();

			CVIBuffer_Terrain* pTerrain = static_cast<CVIBuffer_Terrain*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"), 0));
			if (pTerrain)
			{
				pTerrain->DigGround(Desc.vPos, SHELL_DIG_RADIUS, SHELL_DIG_DEPTH);
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_MODULE_STATE), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_MODULE_STATE);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(MODULE_STATE_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_MODULE_STATE), [this](void* pArg)
		{
			MODULE_STATE_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(MODULE_STATE_DESC));
			m_pGameInstance->Clear_Packet();

			//나의 탱크의 상태 이상 일수도 있음
			CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_PlayerTank"));
			if (pGameObject->Get_ID() == Desc.iTargetID)
			{
					static_cast<CTank*>(pGameObject)->Set_State_Module(Desc.eModule, Desc.eState);
			}
			else
			{
				CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
				if (pLayer == nullptr)
					return;

				//다른 탱크의 상태이상일수도
				for (CGameObject* pGameObject : pLayer->Get_GameObjects())
				{
					if (Desc.iTargetID == pGameObject->Get_ID())
					{
						static_cast<CTank*>(pGameObject)->Set_State_Module(Desc.eModule, Desc.eState);
						break;
					}
				}
			}
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_TAKE_DAMAGE), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_TAKE_DAMAGE);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(TAKEDAMAGE_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_TAKE_DAMAGE), [this](void* pArg)
		{
			TAKEDAMAGE_DESC TakeDamageDesc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&TakeDamageDesc), sizeof(TAKEDAMAGE_DESC));
			m_pGameInstance->Clear_Packet();

			//나의 탱크의 상태 이상 일수도 있음
			CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_PlayerTank"));
			if (pGameObject && pGameObject->Get_ID() == TakeDamageDesc.iTargetID)
			{
				static_cast<CTank*>(pGameObject)->Take_Damage(TakeDamageDesc.fDamage, nullptr, TakeDamageDesc.vFirePos);

				//데미지 인디케이터 띄우자 여기서
				CDamageIndicator::DAMAGE_INDICATOR_DESC		Desc{};

				Desc.fSizeX = 248.f * UI_RATIO;
				Desc.fSizeY = 716.f * UI_RATIO;
				Desc.fX = g_iWinSizeX * 0.5f;
				Desc.fY = g_iWinSizeY * 0.5f;
				Desc.fDepth = DEPTH_BACKGROUND - 0.01f;
				Desc.fRotationPerSec = 1.f;
				Desc.vFirePos = TakeDamageDesc.vFirePos;

				if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageIndicator"),
					ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_DamageIndicator"), &Desc)))
					return;
			}
			else
			{
				CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
				if (pLayer == nullptr)
					return;

				//다른 탱크의 상태이상일수도
				for (CGameObject* pGameObject : pLayer->Get_GameObjects())
				{
					if (pGameObject && TakeDamageDesc.iTargetID == pGameObject->Get_ID())
					{
						static_cast<CTank*>(pGameObject)->Take_Damage(TakeDamageDesc.fDamage, nullptr, TakeDamageDesc.vFirePos);
						break;
					}
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_HIT_MODULE), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_HIT_MODULE);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(HIT_MODULE_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_HIT_MODULE), [this](void* pArg)
		{
			HIT_MODULE_DESC Hit_Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Hit_Desc), sizeof(HIT_MODULE_DESC));
			m_pGameInstance->Clear_Packet();


			//나의 탱크의 상태 이상 일수도 있음
			CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_PlayerTank"));
			if (pGameObject->Get_ID() == Hit_Desc.iTargetID)
			{
				static_cast<CTank*>(pGameObject)->Damage_Module(Hit_Desc.eModule);

				////데미지 인디케이터 띄우자 여기서
				//CDamageIndicator::DAMAGE_INDICATOR_DESC		Desc{};

				//Desc.fSizeX = 248.f * UI_RATIO;
				//Desc.fSizeY = 716.f * UI_RATIO;
				//Desc.fX = g_iWinSizeX * 0.5f;
				//Desc.fY = g_iWinSizeY * 0.5f;
				//Desc.fDepth = DEPTH_BACKGROUND - 0.01f;
				//Desc.fRotationPerSec = 1.f;
				//Desc.vFirePos = Hit_Desc.vFirePos;

				//if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageIndicator"),
				//	ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_DamageIndicator"), &Desc)))
				//	return;
			}
			else
			{
				CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
				if (pLayer == nullptr)
					return;

				//다른 탱크의 상태이상일수도
				for (CGameObject* pGameObject : pLayer->Get_GameObjects())
				{
					if (Hit_Desc.iTargetID == pGameObject->Get_ID())
					{
						static_cast<CTank*>(pGameObject)->Damage_Module(Hit_Desc.eModule);
						break;
					}
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_TANK_MATRIX), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_TANK_MATRIX);

			cout << "CS_TANK_MATRIX" << endl;

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(TANK_MATRIX_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_TANK_MATRIX), [this](void* pArg)
		{
			TANK_MATRIX_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(TANK_MATRIX_DESC));
			m_pGameInstance->Clear_Packet();

			CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
			if (pLayer == nullptr)
				return;

			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				if (Desc.iID == pGameObject->Get_ID())
				{
					pGameObject->Get_Transform()->Set_WorldMatrix(Desc.Matrix_Body);
					CGameObject* pTurret = pGameObject->Find_PartObject(TEXT("Part_Turret"));
					pTurret->Get_Transform()->Set_WorldMatrix(Desc.Matrix_Turret);
					CGameObject* pGun = pTurret->Find_PartObject(TEXT("Part_Gun"));
					pGun->Get_Transform()->Set_WorldMatrix(Desc.Matrix_Gun);
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_MATRIX_BODY), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_MATRIX_BODY);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(MATRIX_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_MATRIX_BODY), [this](void* pArg)
		{
			MATRIX_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(MATRIX_DESC));
			m_pGameInstance->Clear_Packet();

			CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
			if (pLayer == nullptr)
				return;

			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				if (Desc.iID == pGameObject->Get_ID())
				{
					pGameObject->Get_Transform()->Set_WorldMatrix(Desc.Matrix);
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_MATRIX_TURRET), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_MATRIX_TURRET);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(MATRIX_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_MATRIX_TURRET), [this](void* pArg)
		{
			MATRIX_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(MATRIX_DESC));
			m_pGameInstance->Clear_Packet();

			CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
			if (pLayer == nullptr)
				return;

			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				if (Desc.iID == pGameObject->Get_ID())
				{
					CGameObject* pTurret = pGameObject->Find_PartObject(TEXT("Part_Turret"));
					pTurret->Get_Transform()->Set_WorldMatrix(Desc.Matrix);
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_MATRIX_GUN), [this](void* pArg)
		{
			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_MATRIX_GUN);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(MATRIX_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_MATRIX_GUN), [this](void* pArg)
		{
			MATRIX_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(MATRIX_DESC));
			m_pGameInstance->Clear_Packet();

			CLayer* pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Tank"));
			if (pLayer == nullptr)
				return;

			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				if (Desc.iID == pGameObject->Get_ID())
				{
					CGameObject* pTurret = pGameObject->Find_PartObject(TEXT("Part_Turret"));
					CGameObject* pGun = pTurret->Find_PartObject(TEXT("Part_Gun"));
					pGun->Get_Transform()->Set_WorldMatrix(Desc.Matrix);
				}
			}

		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_POSITION), [this](void* pArg)
		{
			if (m_pGameInstance->Get_Network_Status() == NETWORK_STATUS::DISCONNECTED)
				return;

			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_POSITION);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));

			POSITION_DESC* Position_Desc = static_cast<POSITION_DESC*>(pArg);
			Position_Desc->iID = m_pGameInstance->Get_ID();

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(Position_Desc), sizeof(POSITION_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_POSITION), [this](void* pArg)
		{
			POSITION_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(POSITION_DESC));

			::cout << Desc.vPos.x << ' ' << Desc.vPos.y << ' ' << Desc.vPos.z << endl;
		})))
		return E_FAIL;
#pragma endregion	

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
#pragma region 컴포넌트
	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::SPHERE))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_VolumeMesh*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_VolumeMesh"),
		CVIBuffer_VolumeMesh::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Trail*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Trail"),
		CVIBuffer_Trail::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPos */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPos"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPos.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_TankSound3D */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_TankSound3D"),
		CSoundController::Create("../Bin/WOT_Resources/Sound/01.TankSound3D/", true))))															
		return E_FAIL;

	/* For.Prototype_Component_Tree3D */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_Tree3D"),
		CSoundController::Create("../Bin/WOT_Resources/Sound/06.Tree3D/", true))))
		return E_FAIL;

	/* For.Prototype_Component_BGM */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_BGM"),
		CSoundController::Create("../Bin/WOT_Resources/Sound/02.BGM/"))))
		return E_FAIL;

	/* For.Prototype_Component_SoundController_Voice */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_Voice"),
		CSoundController::Create("../Bin/WOT_Resources/Sound/03.Voice/"))))
		return E_FAIL;

	/* For.Prototype_Component_SoundController_GUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_GUI"),
		CSoundController::Create("../Bin/WOT_Resources/Sound/04.GUI/"))))
		return E_FAIL;

	/* For.Prototype_Component_SoundController_TankSound2D */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_TankSound2D"),
		CSoundController::Create("../Bin/WOT_Resources/Sound/05.TankSound2D/"))))
		return E_FAIL;

	/* For.Prototype_Component_SoundController_Clock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_Clock"),
		CSoundController::Create("../Bin/WOT_Resources/Sound/08.Clock/"))))
		return E_FAIL;

	/* For.Prototype_Component_SoundController_Missile */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_Missile"),
		CSoundController::Create("../Bin/WOT_Resources/Sound/07.Missile/", true))))
		return E_FAIL;

	/* For.Prototype_Component_SoundController_Plane */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_Plane"),
		CSoundController::Create("../Bin/WOT_Resources/Sound/09.Plane/", true))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_For_Loading()
{
#pragma region 텍스쳐
	/* For.Prototype_Component_Texture_Background_Loading*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Background_Loading"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Loading/Background/99_poland.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Loading_Spinner*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Loading_Spinner"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Loading/Spinner/0.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_WOT_Icon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_WOT_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Loading/Spinner/1.dds"), 1))))
		return E_FAIL;

#pragma endregion


#pragma region 원형 객체
	/* For.Prototype_GameObject_Background_Loading */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Background_Loading"),
		CBackGround_Loading::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Loading_Spinner */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Loading_Spinner"),
		CLoading_Spinner::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_WOT_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_WOT_Icon"),
		CWOT_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eStartLevel)
{
	if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING), CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevel))))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
    __super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	m_pGameInstance->Release_Engine();

	Safe_Release(m_pGameInstance);

#if defined(_DEBUG)
	FreeConsole();
#endif
}
