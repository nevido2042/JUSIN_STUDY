#include "pch.h"
#include "Shop.h"
#include "Define.h"
#include "ItemFactory.h"
#include "EasySword.h"
#include "NormalSword.h"
#include "HardSword.h"

CShop::CShop()
	:m_pPlayer(nullptr)
{
}

CShop::~CShop()
{
	Relase();
}

void CShop::Initialize()
{
	Set_Name("����");
	m_vecItem.push_back(new CEasySword);
	m_vecItem.push_back(new CNormalSword);
	m_vecItem.push_back(new CHardSword);

	for (CItem* pItem : m_vecItem)
	{
		pItem->Initialize();
	}
}

void CShop::Update()
{
}

void CShop::Relase()
{
	for (CItem* pItem : m_vecItem)
	{
		Safe_Delete(pItem);
	}

	m_pPlayer = nullptr;
}

void CShop::Render()
{
	cout << m_szName << endl;
	cout << endl;
	int i(0);
	for (CItem* pItem : m_vecItem)
	{
		++i;
		cout << "[" << i << "]";
		pItem->Render();
		cout << endl;
	}
}

void CShop::OpenShop()
{
	enum { ���� = 1, �Ǹ�, ������ };
	int iInput(0);
	while (true)
	{
		system("cls");
		cout << m_szName << endl;
		cout << endl;
		cout << "1.���� 2.�Ǹ� 3.������" << endl;
		cin >> iInput;
		switch (iInput)
		{
		case ����:
			Open_Buy_Menu();
			break;
		case �Ǹ�:
			Open_Sell_Menu();
			break;
		case ������:
			return;
		}
		
	}
}

void CShop::Open_Buy_Menu()
{
	int iInput(0);
	while (true)
	{
		system("cls");
		Render();
		cout << endl;
		cout << "������: " << m_pPlayer->Get_Money() << endl;
		cout << "�κ��丮 ����";
		cout << m_pPlayer->Get_Inventory()->Get_ItemCount() << "/" << m_pPlayer->Get_Inventory()->Get_MaxSize() << endl;
		cout << endl;
		cout << "������ �������� ��ȣ�� �Է��ϼ���. (������ = 0)" << endl;
		cout << endl;
		cin >> iInput;

		//��Ͽ� ���� ������ �Է½� ���Է� ����
		if (iInput > m_vecItem.size())
		{
			continue;
		}

		if (iInput == 0)
		{
			return;
		}

		//������ Ȯ��
		if (m_pPlayer->Get_Money() < m_vecItem[iInput - 1]->Get_Price())
		{
			cout << "���� ����" << endl;
			cout << "�������� �����մϴ�." << endl;
			system("pause");
			continue;
		}
		//�κ��丮 ���� Ȯ��
		if (m_pPlayer->Get_Inventory()->Is_Full())
		{
			cout << "���� ����" << endl;
			cout << "�κ��丮�� ����á���ϴ�." << endl;
			system("pause");
			continue;
		}
		//������ ����
		m_pPlayer->Add_Money(-m_vecItem[iInput - 1]->Get_Price());

		//�κ��丮�� ������ �߰�
		CItemFactory ItemFactory;
		CItem* CreatedItem = ItemFactory.Create_Item(typeid(*m_vecItem[iInput - 1]).name());
		CreatedItem->Initialize();
		m_pPlayer->Get_Inventory()->Add_Item(CreatedItem);

		cout << "<" << CreatedItem->Get_Name() << ">" << " ���� ����" << endl;
		system("pause");


	}
}

void CShop::Open_Sell_Menu()
{

	int iInput(0);
	while (true)
	{
		system("cls");
		m_pPlayer->Get_Inventory()->Render();
		cout << endl;
		cout << "������: " << m_pPlayer->Get_Money() << endl;
		cout << endl;
		cout << "�Ǹ��� �������� ��ȣ�� �Է��ϼ���. (������ = 0)" << endl;
		cout << endl;
		cin >> iInput;

		//��Ͽ� ���� ������ �Է½� ���Է� ����
		if (iInput > m_pPlayer->Get_Inventory()->Get_ItemCount())
		{
			continue;
		}

		if (iInput == 0)
		{
			return;
		}

		//������ ����
		m_pPlayer->Add_Money(m_vecItem[iInput - 1]->Get_Price());

		//�κ��丮���� ������ ����
		m_pPlayer->Get_Inventory()->Remove_Item(iInput - 1);
	}
}
