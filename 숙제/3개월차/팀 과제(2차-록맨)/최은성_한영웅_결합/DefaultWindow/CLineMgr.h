#pragma once

#include "CLine.h"

class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();

public:
	void	Add_Line(const RECT& _rc);
	void	Release();

public:
	static CLineMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CLineMgr;

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CLineMgr*		m_pInstance;
	list<CLine*>			m_Linelist_Floor;
	list<CLine*>			m_Linelist_Ceiling;
	LINEPOINT				m_tLinePoint[DIR_END];

};

