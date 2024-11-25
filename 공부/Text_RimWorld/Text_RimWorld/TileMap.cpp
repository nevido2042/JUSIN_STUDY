#include "pch.h"
#include "TileMap.h"

CTileMap::CTileMap()
{
	memset(&m_TileMap, 0, sizeof(m_TileMap));
	memset(&m_TileMap_Buffer, 0, sizeof(m_TileMap_Buffer));
}

CTileMap::~CTileMap()
{
}

void CTileMap::Initialize()
{
	memset(&m_TileMap_Buffer, -1, sizeof(m_TileMap_Buffer));
	Visible_Cursor(false);
}

void CTileMap::Update()
{
	Render();
}

void CTileMap::Release()
{
}

void CTileMap::Render()
{
	for (int i = 0; i < MapSize; ++i)
	{
		for (int j = 0; j < MapSize; ++j)
		{
			if (m_TileMap[i][j].Get_Type() != m_TileMap_Buffer[i][j].Get_Type())
			{
				Print_Tile(Pos(j * 2, i), m_TileMap[i][j].Get_Type());
				m_TileMap_Buffer[i][j] = m_TileMap[i][j];
			}
		}
		cout << endl;
	}
	
	GotoXY(Pos(0, 0));
}

void CTileMap::GotoXY(Pos _Pos)
{
	COORD Pos;
	Pos.X = _Pos.iX;
	Pos.Y = _Pos.iY;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void CTileMap::Print_Tile(Pos _Pos, CTile::Type _Type)
{
	GotoXY(_Pos);

	switch (_Type)
	{
	case CTile::NONE:
		cout << "��";
		break;
	case CTile::WALL:
		cout << "��";
		break;
	case CTile::RIM:
		cout << "��";
		break;
	default:
		break;
	}
}

void CTileMap::Visible_Cursor(bool _bool)
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //Ŀ�� ���� (1 ~ 100)
	cursorInfo.bVisible = _bool; //Ŀ�� Visible TRUE(����) FALSE(����)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
