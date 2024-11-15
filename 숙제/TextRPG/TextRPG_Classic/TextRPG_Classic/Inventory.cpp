#include "pch.h"
#include "Inventory.h"
#include "Define.h"

CInventory::CInventory()
	:m_iMaxSize(0)
{
}

CInventory::~CInventory()
{
	Relase();
}

void CInventory::Initialize()
{
	m_iMaxSize = 5;
	m_vecItem.reserve(m_iMaxSize);
}

void CInventory::Update()
{
}

void CInventory::Relase()
{
	for (CItem* pItem : m_vecItem)
	{
		Safe_Delete(pItem);
	}
}

void CInventory::Render()
{
	system("cls");
	for (CItem* pItem : m_vecItem)
	{
		pItem->Render();
	}
	system("pause");
}

void CInventory::Add_Item(CItem* _pItem)
{
	if (m_vecItem.size() == m_iMaxSize)
	{
		cout << "인벤토리가 가득 찼습니다." << endl;
		system("pause");
		return;
	}

	m_vecItem.push_back(_pItem);
}
