#pragma once

#include "Tile.h"
#include "Node.h"

class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	void			Set_TileOption(POS _tPos, TILEOPT _eOpt);
	void			Set_TileOption(float _fX, float _fY, TILEOPT _eOpt);
	void			Set_TileObj(POS _tPos, CObj* _pObj);
	void			Set_TileObj(float _fX, float _fY, CObj* _pObj);

	TILEOPT			Get_TileOption(POS _tPos);
	TILEOPT			Get_TileOption(float _fX, float _fY);
	TILEOPT			Get_TileOption(int _iIndexX, int _iIndexY);

	CObj*		Get_TileObj(POS _tPos);
	CObj*		Get_TileObj(int _iIndexX, int _iIndexY);
	CObj*		Get_TileObj(float _fX, float _fY);

	static int		Get_TileIndex(float _fX, float _fY);
	list<CNode*>	Find_ReachableTiles(POS _tStart, POS _tEnd);
	CObj*			Get_Tile(int _iX, int _iY);
	CObj*			Get_Tile(float _fX, float _fY);

	void	Picking_Tile(POINT pt, int iDrawID, int iOption);
	
	const array<array<CObj*, TILEX>, TILEY>& Get_TileArray() { return m_TileMap; }

	bool IsValidTile(const POS _tPos);
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

	//array<CObj*, TILEX* TILEY>	m_arrTile;
	array<array<CObj*, TILEX>, TILEY>	m_TileMap;

	//올라 갈 수 있는 ...
	//set<CObj*>	m_ReachableTileSet;

	int m_iMinX;
	int m_iMaxX;
	int m_iMinY;
	int m_iMaxY;

};

