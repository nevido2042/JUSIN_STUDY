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

#include "../Default/framework.h"
#include <process.h>
#include "Client_Enum.h"

#include "DebugUtils.h"

#include "PacketType.h"

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






