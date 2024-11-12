#pragma once
#include "resource.h"
#include <wtypes.h>

#define GRID_SIZE 16
#define GRID_WIDTH 100
#define GRID_HEIGHT 50

enum TileType
{
	NONE = 0,
	OBSTACLE,
	START,
	END,
	CHECK,
	NODE,
	LINE
};

typedef struct tagPosition
{
	int iX;
	int iY;
	tagPosition(int _iX = -1, int _iY = -1) : iX(_iX), iY(_iY) {};
	void operator()(int _iX = -1, int _iY = -1)
	{
		iX = _iX;
		iY = _iY;
	}
}POS;

class CTileManager
{

	char m_Tile[GRID_HEIGHT][GRID_WIDTH]; //1 장애물 있음 / 0 장애물 없음

	bool m_bErase;
	bool m_bDrag;

	POS m_StartPos;
	POS m_EndPos;

	POS m_OldPos;
	POS m_CurrentPos;
	POS m_OldTile;

	RECT m_DCRect;
	HDC m_hDC;

	HBITMAP m_hDCBitmap;
	HBITMAP m_hDCBitmap_old;

	HBRUSH m_hTileBrush;
	HPEN m_hGridPen;

	HBRUSH m_hStartBrush;
	HBRUSH m_hEndBrush;

public:
	void Initialize();
public:
	bool Get_IsErase()
	{
		return m_bErase;
	}
	bool Get_IsDrag()
	{
		return m_bDrag;
	}
	POS Get_OldPos()
	{
		return m_OldPos;
	}
	void Set_OldPos(POS _Pos)
	{
		m_OldPos = _Pos;
	}
	POS Get_CurrentPos()
	{
		return m_CurrentPos;
	}
	void Set_CurrentPos(POS _Pos)
	{
		m_CurrentPos = _Pos;
	}
	void Set_OldTile(POS _Pos)
	{
		m_OldTile.iX = _Pos.iX / GRID_SIZE;
		m_OldTile.iY = _Pos.iY / GRID_SIZE;
	}
	POS Get_StartPos()
	{
		return m_StartPos;
	}
	void Set_StartPos(POS _Pos)
	{
		m_StartPos = _Pos;
	}
	POS Get_EndPos()
	{
		return m_EndPos;
	}
	void Set_EndPos(POS _Pos = POS(-1, -1))
	{
		m_EndPos = _Pos;
	}
	int Get_Tile(int _iX, int _iY)
	{
		return m_Tile[_iY][_iX];
	}
	void Set_Tile(int _iX, int _iY, int _iType)
	{
		m_Tile[_iY][_iX] = _iType;
	}

	void Set_IsDrag(bool _bool)
	{
		m_bDrag = _bool;
	}
	void Set_IsErase(bool _bool)
	{
		m_bErase = _bool;
	}
	HDC& Get_DC()
	{
		return m_hDC;
	}
	void Set_DC(HDC _HDC)
	{
		m_hDC = _HDC;
	}
	HBITMAP& Get_DCBitmap_Old()
	{
		return m_hDCBitmap_old;
	}
	void Set_DCBitmap_Old(HBITMAP _HBITMAP)
	{
		m_hDCBitmap_old = _HBITMAP;
	}
	HBITMAP& Get_DCBitmap()
	{
		return m_hDCBitmap;
	}
	void Set_DCBitmap(HBITMAP _HBITMAP)
	{
		m_hDCBitmap = _HBITMAP;
	}
	RECT& Get_DCRect()
	{
		return m_DCRect;
	}
public:
	void RenderTile();
	void RenderGrid();
};

