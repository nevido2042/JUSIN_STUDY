#include "pch.h"
#include "MainGame.h"
#include "Define.h"
#include "Player.h"
#include "EasyDungeon.h"
#include "NormalDungeon.h"
#include "HardDungeon.h"
#include "ItemList.h"

CMainGame::CMainGame()
	:m_pPlayer(nullptr), m_pDungeon(nullptr), m_pShop(nullptr)
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
	Safe_Delete(m_pDungeon);
	Safe_Delete(m_pShop);

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
			m_pPlayer->Set_MaxHP(15);
			m_pPlayer->Set_STR(5);
			m_pPlayer->Set_Weapon(new CEasySword);
			m_pPlayer->Set_Armor(new CEasyArmor);

			m_pPlayer->Initialize();


			break;
		case 마법사:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("마법사");
			m_pPlayer->Set_MaxHP(9);
			m_pPlayer->Set_STR(1);
			m_pPlayer->Set_Weapon(new CEasySword);
			m_pPlayer->Set_Armor(new CEasyArmor);

			m_pPlayer->Initialize();


			break;
		case 도적:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("도적");
			m_pPlayer->Set_MaxHP(9);
			m_pPlayer->Set_STR(3);
			m_pPlayer->Set_Weapon(new CEasySword);
			m_pPlayer->Set_Armor(new CEasyArmor);

			m_pPlayer->Initialize();


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
			dynamic_cast<CPlayer*>(m_pPlayer)->Open_Inventory();
			break;
		case 상점:
			m_pShop = new CShop;
			m_pShop->Initialize();
			m_pShop->Set_Player(dynamic_cast<CPlayer*>(m_pPlayer));
			m_pShop->OpenShop();
			Safe_Delete(m_pShop);
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
			Safe_Delete(m_pDungeon);
			break;
		case 중급:
			m_pDungeon = new CNormalDungeon;
			m_pDungeon->Initialize();
			dynamic_cast<CDungeon*>(m_pDungeon)->Start_Battle(m_pPlayer);
			Safe_Delete(m_pDungeon);
			break;
		case 고급:
			m_pDungeon = new CHardDungeon;
			m_pDungeon->Initialize();
			dynamic_cast<CDungeon*>(m_pDungeon)->Start_Battle(m_pPlayer);
			Safe_Delete(m_pDungeon);
			break;
		case 나가기:
			return;
		default:
			break;
		}
	}
}
