#include "pch.h"
#include "CMainGame.h"
#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "BlockMgr.h"
#include "CBmpMgr.h"
#include "CSceneMgr.h"
#include "CWeaponsMenuMgr.h"
#include "CAnimMgr.h"
#include "CUIMgr.h"
//

CMainGame::CMainGame()
	: m_ullTime(GetTickCount64()), m_iFPS(0), m_hDC(nullptr)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
	ullStart_Change_Time = 0;
	m_bShow_Start2 = false;
    ullStart_Change_Time = GetTickCount64(); // 시작 시간 저장
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
    m_hDC = GetDC(g_hWnd);
    CSceneMgr::Get_Instance()->Initialize();
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/player_all_flip_2x.bmp", L"Player_Left");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/player_all_2x.bmp", L"Player_Right");
    CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/Start/Bmp/Start_1.bmp", L"Start_1");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/Start/Bmp/Start_2.bmp", L"Start_2");
}


void CMainGame::Update()
{
   
		CSceneMgr::Get_Instance()->Update();// 추가한거
        CKeyMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
        CSceneMgr::Get_Instance()->Late_Update();
		CKeyMgr::Get_Instance()->Update();
	
}

void CMainGame::Render()
{
#pragma region FPS 출력
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
    CBlockMgr::Get_Instance()->Render(hMemDC);

    BitBlt(m_hDC,
        0, 0, WINCX, WINCY,
        hMemDC,
        0,
        0,
        SRCCOPY);
}


void CMainGame::Release()
{
    CAnimMgr::Destroy_Instance();

    CWeaponsMenuMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();   
	CLineMgr::Destroy_Instance();
	CBlockMgr::Destroy_Instance();
	CObjMgr::DestroyInstance();
    CUIMgr::Destroy_Instance();

	ReleaseDC(g_hWnd, m_hDC);
}
