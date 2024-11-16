#include "pch.h"
#include "Player.h"
#include "Define.h"

CPlayer::CPlayer()
	:m_pInventory(nullptr)
{

}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_iMoney = 100;
	CPawn::Initialize();

	m_pInventory = new CInventory;
	m_pInventory->Initialize();
	m_pInventory->Set_Owner(this);
}

void CPlayer::Update()
{
}

void CPlayer::Release()
{
	Safe_Delete(m_pInventory);
}

void CPlayer::Open_Inventory()
{
	int iInput(0);
	while (true)
	{
		system("cls");
		Render();
		cout << TWO_LINE << endl;
		Render_Equipment();
		cout << TWO_LINE << endl;
		m_pInventory->Render();
		cout << endl;
		cout << "사용할 아이템을 선택하세요.(나가기 = 0)" << endl;
		cin >> iInput;

		//아이템 목록에 없는 번호를 누르면 다시 입력받기
		if (iInput > m_pInventory->Get_ItemCount())
		{
			continue;
		}

		if (iInput == 0)
		{
			return;
		}

		//아이템 사용(장착)
		m_pInventory->Use_Item(iInput - 1);

		//인벤토리에서 아이템 제거
		m_pInventory->Pop_Item(iInput - 1);
	}
}
