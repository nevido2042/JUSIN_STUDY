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
	if (m_vecItem.size() == 0)
	{
		cout << "아이템이 없습니다." << endl;
		return;
	}

	int i(0);
	for (CItem* pItem : m_vecItem)
	{
		++i;
		cout << "[" << i << "]";
		pItem->Render();
		cout << endl;
	}
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

void CInventory::Remove_Item(int _iIndex)
{
	Safe_Delete(m_vecItem[_iIndex]);
	m_vecItem.erase(m_vecItem.begin() + _iIndex);
}
