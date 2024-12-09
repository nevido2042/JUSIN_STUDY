#include "pch.h"
#include "CStage_Hero.h"
#include "CScene.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "BlockMgr.h"
#include "CWall.h"
#include "CScrollMgr.h"
#include "Boss_IceMan.h"

CStage_Hero::CStage_Hero()
{
}

CStage_Hero::~CStage_Hero()
{
}

void CStage_Hero::Initialize()
{
	//모든 매니저들 불러서 모든것들 다 이니셜라이즈
	//CLineMgr::Get_Instance()->Initialize();
	//CBlockMgr::Get_Instance()->Initialize();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");
	
	CObjMgr::Get_Instance()->Add_Object
	(OBJ_BOSS, CAbstractFactory<CBoss_IceMan>::Create(500.f, 100.f, DIR_END));
}

void CStage_Hero::Update()
{
	if (CScrollMgr::Get_Instance()->Update())
		return;

	// 모든 오브젝트들을 순회하면서 업데이트
	CObjMgr::Get_Instance()->Update();
}

void CStage_Hero::LateUpdate()
{
	if (0 != CScrollMgr::Get_Instance()->Get_Dir())
		return;
	CObjMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Update();
}

void CStage_Hero::Release()
{
	//씬이 끝났을때
	for (UINT i = 0; i < (UINT)GROUP_TYPE::OBJ_END; ++i)
	{
		for (size_t j = 0; j < m_VecObj[i].size(); ++i)
		{
			delete m_VecObj[i][j];
		}

	}
}

void CStage_Hero::Render(HDC _hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	HDC		hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground");

	BitBlt(hMemDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	//CLineMgr::Get_Instance()->Render(m_hDC);
	CBlockMgr::Get_Instance()->Render(hMemDC);

	CObjMgr::Get_Instance()->Render(hMemDC);

	BitBlt(_hDC,
		0, 0, WINCX, WINCY,
		hMemDC,
		0,
		0,
		SRCCOPY);
}

