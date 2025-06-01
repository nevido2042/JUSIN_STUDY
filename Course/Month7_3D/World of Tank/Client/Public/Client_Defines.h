#pragma once

/* 직교투영, 카메라, 객체만듬, 후처리 쉐이딩, 블러, 그림자, 소프트이펙트, 
픽셀피킹, 모델 로딩, 애니메이팅, 파티클, 인스턴싱, 스프레팅쉐이더 */

/* 특정 헤더파일에 소속되지 않는 전역정보들을 선언해두는 역활 */
/* 기타 다른 정의파일(.h)들을 인클루드하는 역활. */

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

// ------ 여기서부터 ImGui 전용 안전 구역 -------
#undef new
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#define IMGUI_API
#include "ImGuizmo.h"

#ifdef _DEBUG
#define new DBG_NEW
#endif // DEBUG
// ------ 안전 구역 끝 --------

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

#define TERRAIN_SIZE			128.f	//지형 크기
#define TERRAIN_OFFSET_WIDTH	5.0f	//수평
#define TERRAIN_OFFSET_HEIGHT	1.f		//높이


