#include "pch.h"
#include "CScene_Start.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CLineMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CSceneMgr.h"
#include "BlockMgr.h"

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Initialize(CObj* _pPlayer)
{
	
}

void CScene_Start::Update() // ��������
{
	//case STAGE_HONG: // 1�� ȫ����
//    filePath = L"../Data/Line_Hong.dat";
//    break;
//case STAGE_SILVER:// 2�� ������
//    filePath = L"../Data/Line_Silver.dat";
//    break;
//case STAGE_HERO://3�� �ѿ���
//    filePath = L"../Data/Line_Hero.dat";
//    break;
//case STAGE_CHAE://4�� ��ä��
//    filePath = L"../Data/Line_Chae.dat";
//    break;
//case STAGE_MOO://5�� �ӹ���
//    filePath = L"../Data/Line_Moo.dat";
//    break;

	if (GetAsyncKeyState('1'))
	{
		//g_CurrentStage =(int)SCENE_TYPE::STAGE_Hong;
   		CSceneMgr::Get_Instance()->Set_Scene(SCENE_TYPE::STAGE_Hong); // ���� ���������� ��ȯ
		//CLineMgr::Get_Instance()->Load_Line(L"../Data/Line_Hong.dat");
		CBlockMgr::Get_Instance()->Load_Block(L"../Data/Block_Hong.dat");
	}
	if (GetAsyncKeyState('2'))
	{
		//g_CurrentStage = (int)SCENE_TYPE::STAGE_Silver;
		CSceneMgr::Get_Instance()->Set_Scene(SCENE_TYPE::STAGE_Silver); // ���� ���������� ��ȯ		
		//CLineMgr::Get_Instance()->Load_Line(L"../Data/Line_Silver.dat");
		CBlockMgr::Get_Instance()->Load_Block(L"../Data/Block_Silver.dat");


	}
	if (GetAsyncKeyState('3'))
	{
		//g_CurrentStage = (int)SCENE_TYPE::STAGE_Hero;
		CSceneMgr::Get_Instance()->Set_Scene(SCENE_TYPE::STAGE_Hero); // ���� ���������� ��ȯ	
		//CLineMgr::Get_Instance()->Load_Line(L"../Data/Line_Hero.dat");
		CBlockMgr::Get_Instance()->Load_Block(L"../Data/Block_Hero.dat");

	}
	if (GetAsyncKeyState('4'))
	{
		//g_CurrentStage = (int)SCENE_TYPE::STAGE_Chae;
		CSceneMgr::Get_Instance()->Set_Scene(SCENE_TYPE::STAGE_Chae); // ä�� ���������� ��ȯ
		//CLineMgr::Get_Instance()->Load_Line(L"../Data/Line_Chae.dat");
		CBlockMgr::Get_Instance()->Load_Block(L"../Data/Block_Chae.dat");

	}
	if (GetAsyncKeyState('5'))
	{
		//g_CurrentStage = (int)SCENE_TYPE::STAGE_Moo;
		CSceneMgr::Get_Instance()->Set_Scene(SCENE_TYPE::STAGE_Moo); // ���� ���������� ��ȯ
		//CLineMgr::Get_Instance()->Load_Line(L"../Data/Line_Moo.dat");
		CBlockMgr::Get_Instance()->Load_Block(L"../Data/Block_Moo.dat");

	}

}

void CScene_Start::Late_Update()
{
}

void CScene_Start::Render(HDC _hDC) //�Ű� x
{
	Rectangle(_hDC, 0, 0, WINCX, WINCY);
	HFONT newFont = CreateFont(50, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	HFONT oldFont = (HFONT)SelectObject(_hDC, newFont);
	TCHAR szTitleText[32];
	wsprintf(szTitleText, L"Game Start");
	TextOut(_hDC, WINCX / 2 - 130, WINCY / 2 - 150, szTitleText, lstrlen(szTitleText));
	SelectObject(_hDC, oldFont);
	DeleteObject(newFont);

	newFont = CreateFont(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	oldFont = (HFONT)SelectObject(_hDC, newFont);
	TCHAR szStartText[32];
	wsprintf(szStartText, L"Space: Game Start");
	TextOut(_hDC, WINCX / 2 - 120, WINCY / 2 + 170, szStartText, lstrlen(szStartText));
	SelectObject(_hDC, oldFont);
	DeleteObject(newFont);
}

void CScene_Start::Release()
{
}


void CScene_Start::Exit()
{
	
}
