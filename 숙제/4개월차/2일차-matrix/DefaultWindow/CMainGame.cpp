#include "pch.h"
#include "CMainGame.h"
#include "KeyMgr.h"

CMainGame::CMainGame()
	: m_pPlayer(nullptr), m_pMonster(nullptr), m_dwTime(GetTickCount()), m_iFPS(0)
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

	if (!m_pPlayer)
	{
		m_pPlayer = new CTank;
		m_pPlayer->Initialize();
	}

	/*if (!m_pMonster)
	{
		m_pMonster = new CMonster;
		m_pMonster->Initialize();
	}

	dynamic_cast<CMonster*>(m_pMonster)->Set_Player(m_pPlayer);*/
}

void CMainGame::Update()
{
	m_pPlayer->Update();
	//m_pMonster->Update();

}

void CMainGame::Render()
{
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}

	Rectangle(m_hDC, 0, 0, WINCX, WINCY);

	m_pPlayer->Render(m_hDC);
	//m_pMonster->Render(m_hDC);

}

void CMainGame::Release()
{

	CKeyMgr::Destroy_Instance();

	Safe_Delete<CObj*>(m_pPlayer);
	//Safe_Delete<CObj*>(m_pMonster);

	ReleaseDC(g_hWnd, m_hDC);
}
