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

namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;


using namespace Client;



/* 1. �˽ѳ�(��). */
/* 1. �����ǽİ�.(��) */
/* 1. ��õ����.(��) */
/* 1. ���̻�, ���̻� */
/* 1. 5�� ȣ�λ���.(��) */
/* 1. �߰׸�����2��.(��) */
/* 1. ���ѻڶ���.(��) */
/* 1. ����.(��) */
/* 1. �ڽ�����.(��, ��) */
/* 1. ���Ʈ��. */
/* 1. ������Ӿ�ī���� */
/* 1. ����� ���� */
/* 1. ����� Ż��. (���, �ְ� ������, �ְ� ī��, ) */






