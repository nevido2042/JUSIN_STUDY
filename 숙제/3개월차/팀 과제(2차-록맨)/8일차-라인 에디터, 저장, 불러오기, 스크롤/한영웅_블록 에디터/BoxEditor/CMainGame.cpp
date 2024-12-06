#include "pch.h"
#include "CMainGame.h"
#include "BlockMgr.h"
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

	// for (int i = 0; i < 7; ++i)
	// {
	// 	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster>::Create(float(rand() % WINCX), float(rand() % WINCY), 0.f));
	// 	CObjMgr::Get_Instance()->Get_LastMonster()->Set_Target(CObjMgr::Get_Instance()->Get_Player());
	// }
	CBlockMgr::Get_Instance()->Initialize();
}

void CMainGame::Update()
{
	CBlockMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CKeyMgr::Get_Instance()->Update();

	CBlockMgr::Get_Instance()->Late_Update();

}


void CMainGame::Render()
{
#pragma region  FPS Ãâ·Â
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

	CBlockMgr::Get_Instance()->Render(m_hDC);
}

void CMainGame::Release()
{
	CScrollMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CBlockMgr::Destroy_Instance();

	ReleaseDC(g_hWnd, m_hDC);
}
