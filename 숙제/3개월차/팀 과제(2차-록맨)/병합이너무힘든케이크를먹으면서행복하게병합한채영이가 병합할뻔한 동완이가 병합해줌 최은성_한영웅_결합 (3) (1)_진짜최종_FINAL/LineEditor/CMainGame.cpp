#include "pch.h"
#include "CMainGame.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"

CMainGame::CMainGame()
	: m_ullTime(GetTickCount64()), m_iFPS(0), m_hDC(nullptr)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	CLineMgr::Get_Instance()->Initialize();
}

void CMainGame::Update()
{
	CLineMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CKeyMgr::Get_Instance()->Update();
	CLineMgr::Get_Instance()->Late_Update();
}


void CMainGame::Render()
{
#pragma region  FPS ���
	++m_iFPS;

	if (m_ullTime + 1000 < GetTickCount64())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_ullTime = GetTickCount64();
	}
#pragma endregion
	
	Rectangle(m_hDC, 0, 0, WINCX, WINCY);

	CLineMgr::Get_Instance()->Render(m_hDC);


}

void CMainGame::Release()
{
	CScrollMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CLineMgr::Destroy_Instance();
	ReleaseDC(g_hWnd, m_hDC);
}
