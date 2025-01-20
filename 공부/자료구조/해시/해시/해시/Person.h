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
	int m_iSSN;				//주민등록번호
	char m_szName[STR_LEN];	//이름
	char m_szAddr[STR_LEN];	//주소
};



