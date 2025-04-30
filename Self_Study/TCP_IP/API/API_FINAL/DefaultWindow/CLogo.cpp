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

	m_hVideo = MCIWndCreate(g_hWnd,		// 부모 창의 핸들
							nullptr,	// MCI 윈도우를 사용하는 인스턴스 핸들
							WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, // 창 모양 설정(자식 창 | 그 즉시 화면 출력 | 플레이바 생성 안함)
							L"../Video/Wildlife.wmv");	// 재생할 파일의 이름을 포함한 경로


	// MoveWindow : 동영상을 재생할 크기를 설정
	MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, FALSE);

	// 동영상 재생
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
