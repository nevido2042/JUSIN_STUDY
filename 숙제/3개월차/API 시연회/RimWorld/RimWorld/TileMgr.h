#pragma once

#include "Tile.h"

class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	void	Initialize() ;
	int		Update() ;
	void	Late_Update() ;
	void	Render(HDC hDC) ;
	void	Release() ;

	void		Picking_Tile(POINT pt, int iDrawID, int iOption);

public:
	static CTileMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CTileMgr;

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
	static CTileMgr* m_pInstance;

	array<CObj*, TILEX* TILEY>	m_arrTile;

};

