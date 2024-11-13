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

public:
	CMap(int _iWidth, int _iHeight);
public:
	void Initialize();
	void Update();
	//void Release();
private:
	void Visible_Cursor(bool _bool);
	void GotoXY(int _iX, int _iY);
	void Print_Tile(int _iX, int _iY, TILE_TYPE _Type);
public:
	void Render();
	void Change_Tile(int _iX, int _iY, TILE_TYPE _Type);
};

