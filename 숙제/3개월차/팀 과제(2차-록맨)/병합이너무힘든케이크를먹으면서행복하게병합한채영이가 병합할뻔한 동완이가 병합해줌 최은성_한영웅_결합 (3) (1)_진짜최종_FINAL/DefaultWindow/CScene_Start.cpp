#include "pch.h"
#include "CScene_Start.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CLineMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CSceneMgr.h"
#include "BlockMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"

CScene_Start::CScene_Start() : m_bSelectScene(false), m_iRectPos(0)
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/Start_1.bmp", L"Start_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/Choice.bmp", L"Choice");
}


void CScene_Start::Update() // 스테이지
{
	if (m_bSelectScene)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
		{
			m_iRectPos++;
			Update_RectPos();
		}
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
		{
			m_iRectPos--;
			Update_RectPos();
		}
		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			Select_Scene();
		}
	}
	else
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			m_bSelectScene = true;
			Update_RectPos();
		}
	}
}

void CScene_Start::Late_Update()
{


}
	
void CScene_Start::Render(HDC _hDC)
{
	HDC		hBckDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	HDC		hStartDC; 

	if (m_bSelectScene)
	{
		hStartDC = CBmpMgr::Get_Instance()->Find_Image(L"Choice");

		BitBlt(hBckDC,
			0, 0, WINCX, WINCY,
			hStartDC,
			0,
			0,
			SRCCOPY);

		HPEN newPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
		HPEN oldPen = (HPEN)SelectObject(hBckDC, newPen);

		MoveToEx(hBckDC, m_tSelectRect.left, m_tSelectRect.top, nullptr);
		LineTo(hBckDC, m_tSelectRect.right, m_tSelectRect.top);
		LineTo(hBckDC, m_tSelectRect.right, m_tSelectRect.bottom);
		LineTo(hBckDC, m_tSelectRect.left, m_tSelectRect.bottom);
		LineTo(hBckDC, m_tSelectRect.left, m_tSelectRect.top);

		SelectObject(hBckDC, oldPen);
		DeleteObject(newPen);
	}
	else
		hBckDC = CBmpMgr::Get_Instance()->Find_Image(L"Start_1");

	BitBlt(_hDC,
		0, 0, WINCX, WINCY,
		hBckDC,
		0,
		0,
		SRCCOPY);
}

void CScene_Start::Release()
{
}
void CScene_Start::LateUpdate()
{
}

void CScene_Start::Update_RectPos()
{
	switch (m_iRectPos % 5)
	{
	case 0:
		m_tSelectRect.left = 245;
		m_tSelectRect.top = 79;
		break;
	case 1:
		m_tSelectRect.left = 444;
		m_tSelectRect.top = 77;
		break;
	case 2:
		m_tSelectRect.left = 545;
		m_tSelectRect.top = 240;
		break;
	case 3:
		m_tSelectRect.left = 243;
		m_tSelectRect.top = 401;
		break;
	case 4:
		m_tSelectRect.left = 144;
		m_tSelectRect.top = 241;
		break;
	}
	m_tSelectRect.right = m_tSelectRect.left + 107;
	m_tSelectRect.bottom = m_tSelectRect.top + 84;
}

void CScene_Start::Select_Scene()
{
	switch (m_iRectPos % 5)
	{
	case 0: 
		//g_CurrentStage = (int)SCENE_TYPE::STAGE_Silver;
		CSceneMgr::Get_Instance()->Set_Scene(SCENE_TYPE::STAGE_Silver); // 은성 스테이지로 전환		
		//CLineMgr::Get_Instance()->Load_Line(L"../Data/Line_Silver.dat");
		CBlockMgr::Get_Instance()->Load_Block(L"../Data/Block_Silver.dat");
		break;
	case 1:
		//g_CurrentStage = (int)SCENE_TYPE::STAGE_Chae;
		CSceneMgr::Get_Instance()->Set_Scene(SCENE_TYPE::STAGE_Chae); // 채영 스테이지로 전환
		//CLineMgr::Get_Instance()->Load_Line(L"../Data/Line_Chae.dat");
		CBlockMgr::Get_Instance()->Load_Block(L"../Data/Block_Chae.dat");
		break;
	case 2:
		//g_CurrentStage = (int)SCENE_TYPE::STAGE_Hero;
		CSceneMgr::Get_Instance()->Set_Scene(SCENE_TYPE::STAGE_Hero); // 영웅 스테이지로 전환	
		//CLineMgr::Get_Instance()->Load_Line(L"../Data/Line_Hero.dat");5
		CBlockMgr::Get_Instance()->Load_Block(L"../Data/Block_Hero.dat");
		break;
	case 3:
		CSceneMgr::Get_Instance()->Set_Scene(SCENE_TYPE::STAGE_Hong); // 동완 스테이지로 전환
		//CLineMgr::Get_Instance()->Load_Line(L"../Data/Line_Hong.dat");
		CBlockMgr::Get_Instance()->Load_Block(L"../Data/Block_Hong.dat");
		break;
	case 4:
		//g_CurrentStage = (int)SCENE_TYPE::STAGE_Moo;
		CSceneMgr::Get_Instance()->Set_Scene(SCENE_TYPE::STAGE_Moo); // 무결 스테이지로 전환
		//CLineMgr::Get_Instance()->Load_Line(L"../Data/Line_Moo.dat");
		CBlockMgr::Get_Instance()->Load_Block(L"../Data/Block_Moo.dat");
		break;
	case 5:
		break;
	}
}



