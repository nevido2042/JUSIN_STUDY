#include "pch.h"
#include "MainGame.h"
#include "Define.h"
#include "Player.h"

CMainGame::CMainGame()
	:m_pPlayer(nullptr)
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
	cout << "������ �����ϼ���." << endl;
	cout << "1.���� 2.������ 3.���� 4.��������" << endl;
	cin >> iInput;

	while (true)
	{
		switch (iInput)
		{
		case ����:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("����");
			Select_Menu();

			break;
		case ������:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("������");
			Select_Menu();

			break;
		case ����:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("����");
			Select_Menu();

			break;
		case ��������:
			return;
		default:
			break;
		}
	}
}

void CMainGame::Select_Menu()
{
}
