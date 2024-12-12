#pragma once

#include "Tile.h"

class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	void	Set_TileOption(float _fX, float _fY, TILEOPT _eOpt);
	void	Picking_Tile(POINT pt, int iDrawID, int iOption);

public:
	void	Initialize() ;
	int		Update() ;
	void	Late_Update() ;
	void	Render(HDC hDC) ;
	void	Release() ;

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

