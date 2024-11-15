#include "pch.h"
#include "MainGame.h"
#include "Define.h"
#include "Player.h"
#include "EasyDungeon.h"
#include "NormalDungeon.h"
#include "HardDungeon.h"

CMainGame::CMainGame()
	:m_pPlayer(nullptr), m_pDungeon(nullptr)
{
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	Select_Class();
}

void CMainGame::Release()
{
	Safe_Delete(m_pPlayer);
}

void CMainGame::Select_Class()
{
	enum
	{
		전사 = 1, 마법사, 도적, 게임종료
	};

	int iInput(0);


	while (true)
	{
		system("cls");

		cout << "직업을 선택하세요." << endl;
		cout << endl;
		cout << "1.전사 2.마법사 3.도적 4.게임종료" << endl;
		cin >> iInput;

		switch (iInput)
		{
		case 전사:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("전사");
			dynamic_cast<CPawn*>(m_pPlayer)->Set_MaxHP(15);
			dynamic_cast<CPawn*>(m_pPlayer)->Set_STR(5);
			dynamic_cast<CPawn*>(m_pPlayer)->Initialize();


			break;
		case 마법사:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("마법사");
			dynamic_cast<CPawn*>(m_pPlayer)->Set_MaxHP(9);
			dynamic_cast<CPawn*>(m_pPlayer)->Set_STR(1);
			dynamic_cast<CPawn*>(m_pPlayer)->Initialize();


			break;
		case 도적:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("도적");
			dynamic_cast<CPawn*>(m_pPlayer)->Set_MaxHP(9);
			dynamic_cast<CPawn*>(m_pPlayer)->Set_STR(3);
			dynamic_cast<CPawn*>(m_pPlayer)->Initialize();


			break;
		case 게임종료:
			return;
		default:
			break;
		}

		if (m_pPlayer)
		{
			break;
		}
	}

	Select_Menu();
}

void CMainGame::Select_Menu()
{
	enum { 사냥터 = 1, 인벤토리, 상점, 게임종료 };

	while (true)
	{
		system("cls");
		dynamic_cast<CPlayer*>(m_pPlayer)->Render();

		int iInput(0);
		cout << endl;
		cout << "1.사냥터 2.인벤토리 3.상점 4.게임종료" << endl;
		cin >> iInput;
		switch (iInput)
		{
		case 사냥터:
			Select_Dungeon();
			break;
		case 인벤토리:
			break;
		case 상점:
			break;
		case 게임종료:
			return;
		default:
			break;
		}
	}
	
}

void CMainGame::Select_Dungeon()
{
	enum { 초급 = 1, 중급, 고급, 나가기 };

	while (true)
	{
		system("cls");
		dynamic_cast<CPlayer*>(m_pPlayer)->Render();
		cout << endl;
		int iInput(0);
		cout << "1.초급 2.중급 3.고급 4.나가기" << endl;
		cin >> iInput;
		switch (iInput)
		{
		case 초급:
			m_pDungeon = new CEasyDungeon;
			m_pDungeon->Initialize();
			dynamic_cast<CDungeon*>(m_pDungeon)->Start_Battle(m_pPlayer);
			break;
		case 중급:
			m_pDungeon = new CNormalDungeon;
			m_pDungeon->Initialize();
			dynamic_cast<CDungeon*>(m_pDungeon)->Start_Battle(m_pPlayer);
			break;
		case 고급:
			m_pDungeon = new CHardDungeon;
			m_pDungeon->Initialize();
			dynamic_cast<CDungeon*>(m_pDungeon)->Start_Battle(m_pPlayer);
			break;
		case 나가기:
			return;
		default:
			break;
		}
	}
}
