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
		cout << "����� �������� �����ϼ���.(������ = 0)" << endl;
		cin >> iInput;

		//������ ��Ͽ� ���� ��ȣ�� ������ �ٽ� �Է¹ޱ�
		if (iInput > m_pInventory->Get_ItemCount())
		{
			continue;
		}

		if (iInput == 0)
		{
			return;
		}

		//������ ���(����)
		m_pInventory->Use_Item(iInput - 1);

		//�κ��丮���� ������ ����
		m_pInventory->Pop_Item(iInput - 1);
	}
}
