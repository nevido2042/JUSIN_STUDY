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


	// 몬스터 생성
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOSS, CAbstractFactory<CBoss_ElecMan>::Create());
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CRepeat_Monster>::Create());
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CEggMonster>::Create());

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

