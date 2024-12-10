#include "pch.h"
#include "CStage_Hong.h"
#include "CScene.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "BlockMgr.h"
#include "CWall.h"
#include "CScrollMgr.h"
#include "Boss_FireMan.h"
#include "CMonster_1.h"
#include "CMonster_2.h"


#include "CUIMgr.h"
#include "CHpBar.h"


CStage_Hong::CStage_Hong()
{
}

CStage_Hong::~CStage_Hong()
{
}

void CStage_Hong::Initialize()
{
	//CLineMgr::Get_Instance()->Initialize();
	//CBlockMgr::Get_Instance()->Initialize();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");

	CObjMgr::Get_Instance()->Add_Object(OBJ_BOSS, CAbstractFactory<CBoss_FireMan>::Create());
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster_1>::Create());
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster_2>::Create(300.f, 200.f, 0.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster_2>::Create(100.f, 400.f, 0.f));


	// 2스테이지
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster_1>::Create(1100, 120.f, 0.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster_1>::Create(1100, 320.f, 0.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster_2>::Create(1300.f, 230.f, 0.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster_2>::Create(1300.f, 330.f, 0.f));

	//4번째스테이지(하트다음)
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster_1>::Create(910.f, 1630.f, 0.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster_2>::Create(910.f, 1630.f, 0.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster_2>::Create(1120.f, 1300, 0.f));

	//5스테이지

	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster_1>::Create(1700.f, 1230, 0.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster_2>::Create(1900.f, 1230.f, 0.f));
	


	// UI_Player
	CUIMgr::Get_Instance()->Add_UI(UI_HP_PLAYER, CAbstractFactory<CHpBar>::CreateUI());
}

void CStage_Hong::Update()
{
	if (CScrollMgr::Get_Instance()->Update())
		return;

	CObjMgr::Get_Instance()->Update(); // 얘가 
	CUIMgr::Get_Instance()->Update();
}

void CStage_Hong::LateUpdate()
{

	if (0 != CScrollMgr::Get_Instance()->Get_Dir())
		return;
	CObjMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Late_Update();
}

void CStage_Hong::Release()
{

	for (UINT i = 0; i < (UINT)GROUP_TYPE::OBJ_END; ++i)
	{
		for (size_t j = 0; j < m_VecObj[i].size(); ++i)
		{
			delete m_VecObj[i][j];
		}
	}
}

void CStage_Hong::Render(HDC _hDC)
{

	Rectangle(_hDC, 0, 0, WINCX, WINCY);

	//CLineMgr::Get_Instance()->Render(_hDC); 라인없어서 일단 빼놨음

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	HDC		hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground");

	BitBlt(hMemDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	//CLineMgr::Get_Instance()->Render(m_hDC);
	CBlockMgr::Get_Instance()->Render(hMemDC);

	CObjMgr::Get_Instance()->Render(hMemDC);
	CUIMgr::Get_Instance()->Render(hMemDC);



}
