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

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Initialize(CObj* _pPlayer)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/Start_1.bmp", L"Start_1");
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
	
void CScene_Start::Render(HDC _hDC)
{

	HDC		hBckDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	HDC		hStartDC = CBmpMgr::Get_Instance()->Find_Image(L"Start_1");

	BitBlt(hStartDC, 0, 0, WINCX, WINCY, hBckDC, 0, 0, SRCCOPY);

	BitBlt(_hDC,
		0, 0, WINCX, WINCY,
		hStartDC,
		0,
		0,
		SRCCOPY);
}

void CScene_Start::Release()
{
}

void CScene_Start::Initialize()
{
}

void CScene_Start::LateUpdate()
{
}



