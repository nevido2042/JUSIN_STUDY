#include "pch.h"
#include "Dungeon.h"
#include "Define.h"
#include "Pawn.h"

CDungeon::CDungeon()
	:m_pMonster(nullptr)
{
}

CDungeon::~CDungeon()
{
	Relase();
}

void CDungeon::Render()
{
	
}

void CDungeon::Start_Battle(CObj* m_pPlayer)
{
	enum { °ø°Ý = 1, µµ¸Á };
	int iInput(0);
	while (true)
	{
		system("cls");

		cout << m_szName << endl;
		cout << endl;
		
		m_pPlayer->Render();
		cout << "====================================" << endl;
		m_pMonster->Render();

		cout << endl;

		cout << "1.°ø°Ý 2.µµ¸Á" << endl;
		cin >> iInput;


		CPawn* pPlayer;
		CPawn* pMonster;
		switch (iInput)
		{
		case °ø°Ý:
			if (pPlayer = dynamic_cast<CPawn*> (m_pPlayer))
			{
				pPlayer->Attack(m_pMonster);

			}
			if (pMonster = dynamic_cast<CPawn*> (m_pMonster))
			{
				pMonster->Attack(m_pPlayer);
			}

			//µÑ Áß ÇÏ³ª°¡ Á×À¸¸é ÀüÅõ Á¾·á
			if (pPlayer->Get_HP() <= 0 || pMonster->Get_HP() <= 0)
			{
				if (pPlayer->Get_HP() <= 0)
				{
					pPlayer->Initialize();
				}

				if (pMonster->Get_HP() <= 0)
				{
					pPlayer->Add_Money(pMonster->Get_Money());
				}
				return;
			}

			break;
		case µµ¸Á:
			if (pPlayer = dynamic_cast<CPawn*> (m_pPlayer))
			{
				pPlayer->Add_Money(-pPlayer->Get_Money() >> 1); //µµ¸Á °¡¸é µ· Àý¹Ý ÀÒ¾î¹ö¸²
				return;
			}
		default:
			break;
		}
	}
}

void CDungeon::Initialize()
{
}

void CDungeon::Update()
{
}

void CDungeon::Relase()
{
	Safe_Delete(m_pMonster);
}
