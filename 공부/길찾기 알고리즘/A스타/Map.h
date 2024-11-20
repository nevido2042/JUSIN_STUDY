#pragma once
#include "pch.h"
#include "Define.h"

class CMap
{
	const int m_iHeight;
	const int m_iWidth;
	vector<vector<TILE_TYPE>> m_vecTileMap;
	vector<vector<TILE_TYPE>> m_vecTileMap_Buffer;

	POS m_StartPos;
	POS m_EndPos;

	TILE_TYPE m_CHECK_Color;
	bool m_bPrintPathOnly;

public:
	CMap(int _iWidth, int _iHeight);
public:
	void Initialize();
	void Update();
	//void Release();
public:
	POS Get_StartPos()
	{
		return m_StartPos;
	}
	POS Get_EndPos()
	{
		return m_EndPos;
	}
	int Get_Width()
	{
		return m_iWidth;
	}
	int Get_Height()
	{
		return m_iHeight;
	}
	void Set_PrintPathOnly(bool _bool)
	{
		m_bPrintPathOnly = _bool;
	}
private:
	void Visible_Cursor(bool _bool);
	void GotoXY(int _iX, int _iY);
	void Print_Tile(int _iX, int _iY, TILE_TYPE _Type);
public:
	void Render();
	void Render_Test();
	void Change_Tile(POS _Pos, TILE_TYPE _Type);
	void Change_Tile_Check(POS _Pos);
	TILE_TYPE Get_Tile_Type(POS _Pos);
	void Change_CHECK_Color();
};

