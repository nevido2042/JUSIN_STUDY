#include "pch.h"
#include "CMainGame.h"
#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"

CMainGame::CMainGame()
	: m_dwTime(GetTickCount()), m_iFPS(0), m_hDC(nullptr)
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

	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());

	CObjMgr::Get_Instance()->Add_Object(OBJ_PATH, CAbstractFactory<CPath>::Create());

	if (CPath* pPath = dynamic_cast<CPath*>(CObjMgr::Get_Instance()->Get_Path()))
	{
		for (int i = 0; i < 5; ++i)
		{
			pPath->Add_Point(POINT{ rand() % WINCX,rand() % WINCY });

		}
	}


	/*for (int i = 0; i < 7; ++i)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster>::Create(rand() % WINCX, rand() % WINCY, 0.f));
		CObjMgr::Get_Instance()->Get_LastMonster()->Set_Target(CObjMgr::Get_Instance()->Get_Player());
	}*/
}

void CMainGame::Update()
{

	CObjMgr::Get_Instance()->Update();
	

}

void CMainGame::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();

}


void CMainGame::Render()
{
#pragma region  FPS ���
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}
#pragma endregion
	
	Rectangle(m_hDC, 0, 0, WINCX, WINCY);
	CObjMgr::Get_Instance()->Render(m_hDC);

}

void CMainGame::Release()
{

	CObjMgr::DestroyInstance();

	ReleaseDC(g_hWnd, m_hDC);
}
