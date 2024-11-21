#include "pch.h"
#include "Person.h"

CPerson::CPerson(int _iSSN, const char* _szName, const char* _szAddr)
	:m_iSSN(_iSSN)
{
	memcpy_s(m_szName, sizeof(m_szName), _szName, sizeof(_szName));
	memcpy_s(m_szAddr, sizeof(m_szAddr), _szAddr, sizeof(m_szAddr));
}

void CPerson::Show_Person_Info()
{
	cout << "�ֹε�Ϲ�ȣ: " << m_iSSN << endl;
	cout << "�̸�: " << m_szName << endl;
	cout << "�ּ�: " << m_szAddr << endl;
	cout << endl;
}
