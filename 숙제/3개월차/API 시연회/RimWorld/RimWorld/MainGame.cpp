#include "pch.h"
#include "MainGame.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"

#include "CollisionMgr.h"

#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

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
	CSoundMgr::Get_Instance()->Initialize();

	srand(static_cast<unsigned int>(time(0)));

	m_hDC = GetDC(g_hWnd);
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");

	CSceneMgr::Get_Instance()->Set_Scene(SC_LOGO);

}

void CMainGame::Update()
{

	CSceneMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();

	CKeyMgr::Get_Instance()->Update();
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
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	
	CSceneMgr::Get_Instance()->Render(hMemDC);

	BitBlt(m_hDC,
		0, 0, WINCX, WINCY,
		hMemDC,
		0,
		0,
		SRCCOPY);

}

void CMainGame::Release()
{
	CBmpMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CObjMgr::DestroyInstance();
	CColonyMgr::Destroy_Instance();
	CSoundMgr::Destroy_Instance();
	ReleaseDC(g_hWnd, m_hDC);
}
