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
	Set_Name("상점");
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
	enum { 구매 = 1, 판매, 나가기 };
	int iInput(0);
	while (true)
	{
		system("cls");
		cout << m_szName << endl;
		cout << endl;
		cout << "1.구매 2.판매 3.나가기" << endl;
		cin >> iInput;
		switch (iInput)
		{
		case 구매:
			Open_Buy_Menu();
			break;
		case 판매:
			Open_Sell_Menu();
			break;
		case 나가기:
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
		cout << "소지금: " << m_pPlayer->Get_Money() << endl;
		cout << "인벤토리 공간";
		cout << m_pPlayer->Get_Inventory()->Get_ItemCount() << "/" << m_pPlayer->Get_Inventory()->Get_MaxSize() << endl;
		cout << endl;
		cout << "구매할 아이템의 번호를 입력하세요. (나가기 = 0)" << endl;
		cout << endl;
		cin >> iInput;

		//목록에 없는 아이템 입력시 재입력 받음
		if (iInput > m_vecItem.size())
		{
			continue;
		}

		if (iInput == 0)
		{
			return;
		}

		//소지금 확인
		if (m_pPlayer->Get_Money() < m_vecItem[iInput - 1]->Get_Price())
		{
			cout << "구매 실패" << endl;
			cout << "소지금이 부족합니다." << endl;
			system("pause");
			continue;
		}
		//인벤토리 여유 확인
		if (m_pPlayer->Get_Inventory()->Is_Full())
		{
			cout << "구매 실패" << endl;
			cout << "인벤토리가 가득찼습니다." << endl;
			system("pause");
			continue;
		}
		//소지금 차감
		m_pPlayer->Add_Money(-m_vecItem[iInput - 1]->Get_Price());

		//인벤토리에 아이템 추가
		CItemFactory ItemFactory;
		CItem* CreatedItem = ItemFactory.Create_Item(typeid(*m_vecItem[iInput - 1]).name());
		CreatedItem->Initialize();
		m_pPlayer->Get_Inventory()->Add_Item(CreatedItem);

		cout << "<" << CreatedItem->Get_Name() << ">" << " 구매 성공" << endl;
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
		cout << "소지금: " << m_pPlayer->Get_Money() << endl;
		cout << endl;
		cout << "판매할 아이템의 번호를 입력하세요. (나가기 = 0)" << endl;
		cout << endl;
		cin >> iInput;

		//목록에 없는 아이템 입력시 재입력 받음
		if (iInput > m_pPlayer->Get_Inventory()->Get_ItemCount())
		{
			continue;
		}

		if (iInput == 0)
		{
			return;
		}

		//소지금 증가
		m_pPlayer->Add_Money(m_vecItem[iInput - 1]->Get_Price());

		//인벤토리에서 아이템 제거
		m_pPlayer->Get_Inventory()->Remove_Item(iInput - 1);
	}
}
