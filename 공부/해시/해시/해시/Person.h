#pragma once

#define STR_LEN 50

class CPerson
{
public:
	CPerson(int _iSSN, const char* _szName, const char* _szAddr);
public:
	int Get_SSN()
	{
		return m_iSSN;
	}
	void Show_Person_Info();

public:
	int m_iSSN;				//�ֹε�Ϲ�ȣ
	char m_szName[STR_LEN];	//�̸�
	char m_szAddr[STR_LEN];	//�ּ�
};



