#include "pch.h"
#include "CStage_Moo.h"
#include "CScene.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "BlockMgr.h"
#include "CScrollMgr.h"
#include "CBoss_ElecMan.h"
#include "CRepeat_Monster.h"
#include "CEggMonster.h"
#include "CUIMgr.h"
#include "CHpBar.h"



CStage_Moo::CStage_Moo()
{

}

CStage_Moo::~CStage_Moo()
{
}

void CStage_Moo::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");
	//CObjMgr::Get_Instance()->Get_Player()->Set_Pos(300, -500);

	// 보스 생성
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOSS, CAbstractFactory<CBoss_ElecMan>::Create(1950.f, 430.f));

	// Room 1
	CObjMgr::Get_Instance()->Add_Object(OBJ_GHOSTMONSTER, CAbstractFactory<CRepeat_Monster>::CreateMonster(400, 480, 400, 550));

	// Room 2
	CObjMgr::Get_Instance()->Add_Object(OBJ_GHOSTMONSTER, CAbstractFactory<CRepeat_Monster>::CreateMonster(1300, 230, 1000, 1200));
	CObjMgr::Get_Instance()->Add_Object(OBJ_GHOSTMONSTER, CAbstractFactory<CRepeat_Monster>::CreateMonster(1200, 480, 1000, 1300));

	// Room 3
	CObjMgr::Get_Instance()->Add_Object(OBJ_GHOSTMONSTER, CAbstractFactory<CEggMonster>::CreateMonster(1500, 800, 700, 1200));
	CObjMgr::Get_Instance()->Add_Object(OBJ_GHOSTMONSTER, CAbstractFactory<CEggMonster>::CreateMonster(900, 1100, 700, 1200));
	CObjMgr::Get_Instance()->Add_Object(OBJ_GHOSTMONSTER, CAbstractFactory<CRepeat_Monster>::CreateMonster(1300, 1130, 900, 1350));

	// Room 4 
	CObjMgr::Get_Instance()->Add_Object(OBJ_GHOSTMONSTER, CAbstractFactory<CEggMonster>::CreateMonster(300, 1000, 1000, 1200));
	CObjMgr::Get_Instance()->Add_Object(OBJ_GHOSTMONSTER, CAbstractFactory<CRepeat_Monster>::CreateMonster(400, 1130, 100, 500));

	// Room 5
	CObjMgr::Get_Instance()->Add_Object(OBJ_GHOSTMONSTER, CAbstractFactory<CEggMonster>::CreateMonster(2300, 700, 700, 1200));


	// UI_Player
	CUIMgr::Get_Instance()->Add_UI(UI_HP_PLAYER, CAbstractFactory<CHpBar>::CreateUI());
}

void CStage_Moo::Update()
{
	if (CScrollMgr::Get_Instance()->Update())
		return;

	CObjMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Update();
}

void CStage_Moo::LateUpdate()
{
	if (0 != CScrollMgr::Get_Instance()->Get_Dir())
		return;
	CKeyMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Late_Update();
}

void CStage_Moo::Render(HDC _hDC)
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

void CStage_Moo::Release()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::OBJ_END; ++i)
	{
		for (size_t j = 0; j < m_VecObj[i].size(); ++i)
		{
			delete m_VecObj[i][j];
		}

	}
}

