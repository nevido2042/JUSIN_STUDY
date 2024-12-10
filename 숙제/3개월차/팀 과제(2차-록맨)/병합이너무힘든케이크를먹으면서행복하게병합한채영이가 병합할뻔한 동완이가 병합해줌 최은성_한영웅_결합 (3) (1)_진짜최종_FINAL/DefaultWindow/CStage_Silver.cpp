#include "pch.h"
#include "CStage_Silver.h"
#include "CScene.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "BlockMgr.h"
#include "CWall.h"
#include "CScrollMgr.h"
#include "CBoss_CutMan.h"

#include "CSilver_Mob1.h"
#include "CSilver_Mob2.h"

#include "CUIMgr.h"
#include "CHpBar.h"


CStage_Silver::CStage_Silver(): m_bMopSpawn()
{
}

CStage_Silver::~CStage_Silver()
{
}

void CStage_Silver::Initialize()
{
	//CLineMgr::Get_Instance()->Initialize();
	//CBlockMgr::Get_Instance()->Initialize();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");
	
	CUIMgr::Get_Instance()->Add_UI(UI_HP_PLAYER, CAbstractFactory<CHpBar>::CreateUI());

	//CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBoss_CutMan>::Create(600.f , 380.f, CObjMgr::Get_Instance()->Get_Player()));


	CUIMgr::Get_Instance()->Add_UI(UI_HP_PLAYER, CAbstractFactory<CHpBar>::CreateUI());
	//CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBoss_CutMan>::Create(600.f, 480.f, CObjMgr::Get_Instance()->Get_Player()));
	//CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSilver_Mob2>::Create(600.f, 380.f, CObjMgr::Get_Instance()->Get_Player()));
}

void CStage_Silver::Update()
{
	if (CScrollMgr::Get_Instance()->Update())
		return;


	if (CScrollMgr::Get_Instance()->Get_ScrollX() == -WINCX && m_bMopSpawn[0] == false)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSilver_Mob1>::Create(700.f + WINCX, 430.f, CObjMgr::Get_Instance()->Get_Player()));
		m_bMopSpawn[0] = true;
	}

	if (CScrollMgr::Get_Instance()->Get_ScrollX() == -WINCX && m_bMopSpawn[1] == false)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSilver_Mob1>::Create(580.f + WINCX, 290.f, CObjMgr::Get_Instance()->Get_Player()));
		m_bMopSpawn[1] = true;
	}

	if (CScrollMgr::Get_Instance()->Get_ScrollX() == -WINCX && m_bMopSpawn[2] == false)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSilver_Mob1>::Create(330.f + WINCX, 540.f, CObjMgr::Get_Instance()->Get_Player()));
		m_bMopSpawn[2] = true;
	}

	if (CScrollMgr::Get_Instance()->Get_ScrollX() == -2400 && m_bMopSpawn[3] == false)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSilver_Mob2>::Create(900.f + 2400, 140.f-600.f, CObjMgr::Get_Instance()->Get_Player()));
		m_bMopSpawn[3] = true;
	}

	if (CScrollMgr::Get_Instance()->Get_ScrollX() == -2400 && m_bMopSpawn[4] == false)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSilver_Mob2>::Create(600.f + 2400, 140.f -600.f, CObjMgr::Get_Instance()->Get_Player()));
		m_bMopSpawn[4] = true;
	}

	if (CScrollMgr::Get_Instance()->Get_ScrollX() == -4800 && m_bMopSpawn[9] == false)
	{
		//CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBoss_CutMan>::Create(600.f + 4800, 380.f, CObjMgr::Get_Instance()->Get_Player()));
		m_bMopSpawn[9] = true;
	}


	
	CObjMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Update();
}

void CStage_Silver::LateUpdate()
{
	if (0 != CScrollMgr::Get_Instance()->Get_Dir())
		return;
	CKeyMgr::Get_Instance()->Update();
	//CObjMgr::Get_Instance()->Late_Update();
	CUIMgr::Get_Instance()->Late_Update();
}

void CStage_Silver::Release()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::OBJ_END; ++i)
	{
		for (size_t j = 0; j < m_VecObj[i].size(); ++i)
		{
			delete m_VecObj[i][j];
		}

	}
}


void CStage_Silver::Render(HDC _hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	HDC		hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground");

	BitBlt(hMemDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	//CLineMgr::Get_Instance()->Render(m_hDC);
	CBlockMgr::Get_Instance()->Render(hMemDC);

	CObjMgr::Get_Instance()->Render(hMemDC);

	CUIMgr::Get_Instance()->Render(hMemDC);

	BitBlt(_hDC,
		0, 0, WINCX, WINCY,
		hMemDC,
		0,
		0,
		SRCCOPY);
}

