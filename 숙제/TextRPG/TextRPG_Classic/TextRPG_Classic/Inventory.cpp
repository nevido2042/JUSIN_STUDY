#include "pch.h"
#include "Inventory.h"
#include "Define.h"
#include "Weapon.h";

CInventory::CInventory()
	:m_iMaxSize(0), m_pOwner(nullptr)
{
}

CInventory::~CInventory()
{
	Release();
}

void CInventory::Initialize()
{
	m_iMaxSize = 5;
	m_vecItem.reserve(m_iMaxSize);
}

void CInventory::Update()
{
}

void CInventory::Release()
{
	for (CItem* pItem : m_vecItem)
	{
		Safe_Delete(pItem);
	}
}

void CInventory::Render()
{
	if (m_vecItem.size() == 0)
	{
		cout << "�������� �����ϴ�." << endl;
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
		cout << "�κ��丮�� ���� á���ϴ�." << endl;
		system("pause");
		return;
	}

	m_vecItem.push_back(_pItem);
}

void CInventory::Use_Item(int _iIndex)
{
	if (CWeapon* pWeapon = dynamic_cast<CWeapon*>(m_vecItem[_iIndex]))
	{
		//������ �������� ������ �κ��丮�� ��ȯ
		if (m_pOwner->Get_Weapon())
		{
			Add_Item(m_pOwner->Get_Weapon());
			//�ɷ�ġ ����
			m_pOwner->Add_STR(-m_pOwner->Get_Weapon()->Get_STR());
			
		}

		m_pOwner->Set_Weapon(pWeapon);
		//�ɷ�ġ ����
		m_pOwner->Add_STR(pWeapon->Get_STR());

	}

	//��� �������̸� ����

	//���� �������̸� �Ҹ�
}

void CInventory::Remove_Item(int _iIndex)
{
	Safe_Delete(m_vecItem[_iIndex]);
	m_vecItem.erase(m_vecItem.begin() + _iIndex);
}

void CInventory::Pop_Item(int _iIndex)
{
	m_vecItem.erase(m_vecItem.begin() + _iIndex);
}
