#include "pch.h"
#include "CStage_Moo.h"
#include "CScene.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "BlockMgr.h"
#include "CWall.h"
#include "CScrollMgr.h"

CStage_Moo::CStage_Moo()
{
}

CStage_Moo::~CStage_Moo()
{
}

void CStage_Moo::Initialize()
{
	//CLineMgr::Get_Instance()->Initialize();
	//CBlockMgr::Get_Instance()->Initialize();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");
	for (int i = 0; i < 4; ++i)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, CAbstractFactory<CWall>::Create(float(50), float(i * 100), 0.f));
	}
}

void CStage_Moo::Update()
{
	if (CScrollMgr::Get_Instance()->Update())
		return;

	CObjMgr::Get_Instance()->Update();
}

void CStage_Moo::LateUpdate()
{
	if (0 != CScrollMgr::Get_Instance()->Get_Dir())
		return;
	CKeyMgr::Get_Instance()->Update();
}

void CStage_Moo::Render(HDC _hDC)
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

void CStage_Moo::Release()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::OBJ_END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++i)
		{
			delete m_arrObj[i][j];
		}

	}
}



void CStage_Moo::Exit()
{
}

