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

#include <cstdlib>

#include <filesystem>
#include <fstream>

#include "../Default/framework.h"
#include <process.h>
#include "Client_Enum.h"

#include "DebugUtils.h"

#include "PacketType.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#define IMGUI_API
#include "ImGuizmo.h"

namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;


using namespace Client;

#define DEPTH_BACKGROUND 0.1f

#define PING_TIME 1.f






