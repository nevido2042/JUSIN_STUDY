#include "pch.h"
#include "Equipment.h"

CEquipment::CEquipment()
	:m_iHP(0), m_iSTR(0)
{

}

void CEquipment::Render()
{
	cout << m_szName << endl;
	cout << "추가 체력: " << m_iHP << endl;
	cout << "추가 공격력: " << m_iSTR << endl;
}
