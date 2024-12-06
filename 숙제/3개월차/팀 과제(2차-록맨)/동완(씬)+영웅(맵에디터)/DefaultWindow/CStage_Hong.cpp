#include "pch.h"
#include "CStage_Hong.h"
#include "CScene.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "BlockMgr.h"

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
}

void CStage_Hong::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CStage_Hong::LateUpdate()
{
	CObjMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Update();
}

void CStage_Hong::Release()
{

	for (UINT i = 0; i < (UINT)GROUP_TYPE::OBJ_END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++i)
		{
			delete m_arrObj[i][j];
		}

	}
}

void CStage_Hong::Render(HDC _hDC)
{
	Rectangle(_hDC, 0, 0, WINCX, WINCY);

	//CLineMgr::Get_Instance()->Render(_hDC); ���ξ�� �ϴ� ������

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

void CStage_Hong::Exit()
{
}
