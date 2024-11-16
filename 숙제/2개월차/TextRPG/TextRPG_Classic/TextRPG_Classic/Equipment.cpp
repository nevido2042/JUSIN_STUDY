#include "pch.h"
#include "Equipment.h"

CEquipment::CEquipment()
	:m_iHP(0), m_iSTR(0)
{

}

void CEquipment::Render()
{
	cout << m_szName << endl;
	cout << "추가 체력: " << m_iHP << " ";
	for (int i = 0; i < m_iHP; ++i)
	{
		cout << "■";
	}
	cout << endl;

	cout << "추가 공격력: " << m_iSTR << " ";
	for (int i = 0; i < m_iSTR; ++i)
	{
		cout << "†";
	}
	cout << endl;
	cout << "가격: " << m_iPrice << endl;
}
