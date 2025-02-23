#include "pch.h"
#include "CLogo.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"

CLogo::CLogo()
{
}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Logo/Logo.bmp", L"Logo");

	m_hVideo = MCIWndCreate(g_hWnd,		// �θ� â�� �ڵ�
							nullptr,	// MCI �����츦 ����ϴ� �ν��Ͻ� �ڵ�
							WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, // â ��� ����(�ڽ� â | �� ��� ȭ�� ��� | �÷��̹� ���� ����)
							L"../Video/Wildlife.wmv");	// ����� ������ �̸��� ������ ���


	// MoveWindow : �������� ����� ũ�⸦ ����
	MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, FALSE);

	// ������ ���
	MCIWndPlay(m_hVideo);
}

int CLogo::Update()
{
	if (MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo) ||
		CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CSceneMgr::Get_Instance()->Set_Scene(SC_MENU);
		return 0;
	}


	return 0;
}

void CLogo::Late_Update()
{
}

void CLogo::Render(HDC hDC)
{
	// HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Logo");
	// 
	// BitBlt(hDC,
	// 	0, 0, WINCX, WINCY,
	// 	hMemDC,
	// 	0,
	// 	0,
	// 	SRCCOPY);
}

void CLogo::Release()
{
	MCIWndClose(m_hVideo);
}
