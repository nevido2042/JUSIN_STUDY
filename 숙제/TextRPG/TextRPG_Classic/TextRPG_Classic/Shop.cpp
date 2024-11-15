#include "pch.h"
#include "Shop.h"
#include "Define.h"
#include "EasySword.h"
#include "NormalSword.h"
#include "HardSword.h"

CShop::CShop()
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
		Render();
		cout << endl;
		cout << "1.구매 2.판매 3.나가기" << endl;
		switch (iInput)
		{
		case 구매:
			break;
		}
		
	}
}
