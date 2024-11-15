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
			dynamic_cast<CPawn*>(m_pPlayer)->Set_MaxHP(15);
			dynamic_cast<CPawn*>(m_pPlayer)->Set_STR(5);
			dynamic_cast<CPawn*>(m_pPlayer)->Initialize();


			break;
		case ������:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("������");
			dynamic_cast<CPawn*>(m_pPlayer)->Set_MaxHP(9);
			dynamic_cast<CPawn*>(m_pPlayer)->Set_STR(1);
			dynamic_cast<CPawn*>(m_pPlayer)->Initialize();


			break;
		case ����:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("����");
			dynamic_cast<CPawn*>(m_pPlayer)->Set_MaxHP(9);
			dynamic_cast<CPawn*>(m_pPlayer)->Set_STR(3);
			dynamic_cast<CPawn*>(m_pPlayer)->Initialize();


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
			break;
		case ����:
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
			break;
		case �߱�:
			m_pDungeon = new CNormalDungeon;
			m_pDungeon->Initialize();
			dynamic_cast<CDungeon*>(m_pDungeon)->Start_Battle(m_pPlayer);
			break;
		case ���:
			m_pDungeon = new CHardDungeon;
			m_pDungeon->Initialize();
			dynamic_cast<CDungeon*>(m_pDungeon)->Start_Battle(m_pPlayer);
			break;
		case ������:
			return;
		default:
			break;
		}
	}
}
