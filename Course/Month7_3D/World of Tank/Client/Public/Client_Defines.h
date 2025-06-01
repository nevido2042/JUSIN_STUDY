#pragma once

/* ��������, ī�޶�, ��ü����, ��ó�� ���̵�, ��, �׸���, ����Ʈ����Ʈ, 
�ȼ���ŷ, �� �ε�, �ִϸ�����, ��ƼŬ, �ν��Ͻ�, �������ý��̴� */

/* Ư�� ������Ͽ� �Ҽӵ��� �ʴ� ������������ �����صδ� ��Ȱ */
/* ��Ÿ �ٸ� ��������(.h)���� ��Ŭ����ϴ� ��Ȱ. */

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <locale>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <cstdlib>

#include "../Default/framework.h"
#include <process.h>
#include "Client_Enum.h"

#include "PacketType.h"

// ------ ���⼭���� ImGui ���� ���� ���� -------
#undef new
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#define IMGUI_API
#include "ImGuizmo.h"

#ifdef _DEBUG
#define new DBG_NEW
#endif // DEBUG
// ------ ���� ���� �� --------

namespace Client
{
	//const unsigned int			g_iWinSizeX = 1280;
	//const unsigned int			g_iWinSizeY = 720;
}

//extern _uint g_iWinResizeX;
//extern _uint g_iWinResizeY;

//extern HWND g_hWnd;
extern HINSTANCE g_hInst;


using namespace Client;

#define DEPTH_BACKGROUND 0.1f

#define PING_TIME 1.f

#define GET_GAMEMANAGER static_cast<CGameManager*>(m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Layer_GameManager")))

#define UI_RATIO ((static_cast<_float>(g_iWinSizeX) / g_iBase_WinSizeX + static_cast<_float>(g_iWinSizeY) / g_iBase_WinSizeY) * 0.5f)

#define BASE_FOV 60.f

#define TERRAIN_SIZE			128.f	//���� ũ��
#define TERRAIN_OFFSET_WIDTH	5.0f	//����
#define TERRAIN_OFFSET_HEIGHT	1.f		//����


