#include "pch.h"
#include "Obj.h"

CObj::CObj()
{
	memset(m_szName, 0, sizeof(m_szName));
}

CObj::~CObj()
{
}

void CObj::Set_Name(const char* _szName)
{
	strcpy_s(m_szName, _szName);
}

