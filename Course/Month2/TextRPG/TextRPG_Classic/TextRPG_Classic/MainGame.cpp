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
		���� = 1, ������, ����, ��������
	};

	int iInput(0);


	while (true)
	{
		system("cls");

		cout << "������ �����ϼ���." << endl;
		cout << endl;
		cout << "1.���� 2.������ 3.���� 4.��������" << endl;
		cin >> iInput;

		switch (iInput)
		{
		case ����:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("����");
			m_pPlayer->Set_MaxHP(15);
			m_pPlayer->Set_STR(5);
			m_pPlayer->Set_Weapon(new CEasySword);
			m_pPlayer->Set_Armor(new CEasyArmor);

			m_pPlayer->Initialize();


			break;
		case ������:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("������");
			m_pPlayer->Set_MaxHP(9);
			m_pPlayer->Set_STR(1);
			m_pPlayer->Set_Weapon(new CEasySword);
			m_pPlayer->Set_Armor(new CEasyArmor);

			m_pPlayer->Initialize();


			break;
		case ����:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("����");
			m_pPlayer->Set_MaxHP(9);
			m_pPlayer->Set_STR(3);
			m_pPlayer->Set_Weapon(new CEasySword);
			m_pPlayer->Set_Armor(new CEasyArmor);

			m_pPlayer->Initialize();


			break;
		case ��������:
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
	enum { ����� = 1, �κ��丮, ����, �������� };

	while (true)
	{
		system("cls");
		dynamic_cast<CPlayer*>(m_pPlayer)->Render();

		int iInput(0);
		cout << endl;
		cout << "1.����� 2.�κ��丮 3.���� 4.��������" << endl;
		cin >> iInput;
		switch (iInput)
		{
		case �����:
			Select_Dungeon();
			break;
		case �κ��丮:
			dynamic_cast<CPlayer*>(m_pPlayer)->Open_Inventory();
			break;
		case ����:
			m_pShop = new CShop;
			m_pShop->Initialize();
			m_pShop->Set_Player(dynamic_cast<CPlayer*>(m_pPlayer));
			m_pShop->OpenShop();
			Safe_Delete(m_pShop);
			break;
		case ��������:
			return;
		default:
			break;
		}
	}
	
}

void CMainGame::Select_Dungeon()
{
	enum { �ʱ� = 1, �߱�, ���, ������ };

	while (true)
	{
		system("cls");
		dynamic_cast<CPlayer*>(m_pPlayer)->Render();
		cout << endl;
		int iInput(0);
		cout << "1.�ʱ� 2.�߱� 3.��� 4.������" << endl;
		cin >> iInput;
		switch (iInput)
		{
		case �ʱ�:
			m_pDungeon = new CEasyDungeon;
			m_pDungeon->Initialize();
			dynamic_cast<CDungeon*>(m_pDungeon)->Start_Battle(m_pPlayer);
			Safe_Delete(m_pDungeon);
			break;
		case �߱�:
			m_pDungeon = new CNormalDungeon;
			m_pDungeon->Initialize();
			dynamic_cast<CDungeon*>(m_pDungeon)->Start_Battle(m_pPlayer);
			Safe_Delete(m_pDungeon);
			break;
		case ���:
			m_pDungeon = new CHardDungeon;
			m_pDungeon->Initialize();
			dynamic_cast<CDungeon*>(m_pDungeon)->Start_Battle(m_pPlayer);
			Safe_Delete(m_pDungeon);
			break;
		case ������:
			return;
		default:
			break;
		}
	}
}
