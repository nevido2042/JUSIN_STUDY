#pragma once
#include "Box.h"

class CBoxMgr
{
private:
	CBoxMgr();
	~CBoxMgr();

public:
	//bool	Collision_Box(float _fX, float* pY);

public:
	void	Initialize();
	int		Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();

	void	Save_Box();
	void	Load_Box();

public:
	static CBoxMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CBoxMgr;

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
	static CBoxMgr* m_pInstance;
	list<CBox*>			m_Boxlist;
	//LINEPOINT				m_tLinePoint[DIR_END];

};

