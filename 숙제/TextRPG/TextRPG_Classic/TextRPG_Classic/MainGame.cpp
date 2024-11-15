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
		전사 = 1, 마법사, 도적, 게임종료
	};

	int iInput(0);
	cout << "직업을 선택하세요." << endl;
	cout << "1.전사 2.마법사 3.도적 4.게임종료" << endl;
	cin >> iInput;

	while (true)
	{
		switch (iInput)
		{
		case 전사:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("전사");
			Select_Menu();

			break;
		case 마법사:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("마법사");
			Select_Menu();

			break;
		case 도적:
			m_pPlayer = new CPlayer();
			m_pPlayer->Set_Name("도적");
			Select_Menu();

			break;
		case 게임종료:
			return;
		default:
			break;
		}
	}
}

void CMainGame::Select_Menu()
{
}
