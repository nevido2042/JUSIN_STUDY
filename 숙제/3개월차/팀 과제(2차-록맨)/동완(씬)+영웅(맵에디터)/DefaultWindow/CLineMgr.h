#pragma once

#include "CLine.h"

class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();

public:
	bool	Collision_Line(float _fX, float* pY);

public:
	void	Initialize();

	void	Render(HDC hDC);
	void	Release();

	void	Load_Line();
	void	Load_Line(LPCWSTR _FilePath);

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
	list<CLine*>			m_Linelist;
	LINEPOINT				m_tLinePoint[DIR_END];

};

