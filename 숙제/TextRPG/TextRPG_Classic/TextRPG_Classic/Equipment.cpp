#include "pch.h"
#include "Equipment.h"

CEquipment::CEquipment()
	:m_iHP(0), m_iSTR(0)
{

}

void CEquipment::Render()
{
	cout << m_szName << endl;
	cout << "�߰� ü��: " << m_iHP << endl;
	cout << "�߰� ���ݷ�: " << m_iSTR << endl;
}
