#include "pch.h"
#include "Map.h"

CMap::CMap(int _iWidth, int _iHeight)
	:
	m_vecTileMap(m_iHeight, vector<TILE_TYPE>(m_iWidth, ROAD)),
	m_vecTileMap_Buffer(m_iHeight, vector<TILE_TYPE>(m_iWidth, NONE)),
	m_iWidth(_iWidth),
	m_iHeight(_iHeight),
	m_CHECK_Color(CHECK),
	m_bPrintPathOnly(false)
{
	
}

void CMap::Initialize()
{
	Visible_Cursor(false);
}

void CMap::Update()
{
	Render();
}

void CMap::Visible_Cursor(bool _bool)
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //Ä¿¼­ ±½±â (1 ~ 100)
	cursorInfo.bVisible = _bool; //Ä¿¼­ Visible TRUE(º¸ÀÓ) FALSE(¼û±è)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void CMap::GotoXY(int _iX, int _iY)
{
	COORD Pos;
	Pos.X = _iX;
	Pos.Y = _iY;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void CMap::Print_Tile(int _iX, int _iY, TILE_TYPE _Type)
{
	if (m_bPrintPathOnly)
	{
		if (_Type == CHECK || _Type == CHECK2 || _Type == NODE)
		{
			return;
		}
	}

	GotoXY(_iX * 2, _iY);

	switch (_Type)
	{
	case ROAD:
		cout << "¡à";
		break;
	case WALL:
		cout << "¡á";
		break;
	case START:
		Set_Color(GREEN);
		cout << "¡á";
		Set_Color(GRAY);
		break;
	case END:
		Set_Color(RED);
		cout << "¡á";
		Set_Color(GRAY);
		break;
	case NODE:
		Set_Color(YELLOW);
		cout << "¡á";
		Set_Color(GRAY);
		break;
	case PATH:
		Set_Color(VIOLET);
		cout << "¡á";
		Set_Color(GRAY);
		break;
	case CHECK:
		Set_Color(DARK_BLUE);
		cout << "¡á";
		Set_Color(GRAY);
		break;
	case CHECK2:
		Set_Color(SKYBLUE);
		cout << "¡á";
		Set_Color(GRAY);
		break;
	default:
		break;
	}
}

void CMap::Render()
{
	if (m_bPrintPathOnly)
	{
		return;
	}

	int iOffset(2);

	for (int i = 0; i < m_iHeight; ++i)
	{
		for (int j = 0; j < m_iWidth; ++j)
		{
			if (m_vecTileMap[i][j] != m_vecTileMap_Buffer[i][j])
			{
				Print_Tile(j + iOffset, i + iOffset, m_vecTileMap[i][j]);
				m_vecTileMap_Buffer[i][j] = m_vecTileMap[i][j];
			}
		}
		cout << endl;
	}
	GotoXY(0, 0);
}

void CMap::Render_Test()
{
	int iOffset(2);

	for (int i = 0; i < m_iHeight; ++i)
	{
		for (int j = 0; j < m_iWidth; ++j)
		{
			if (m_vecTileMap[i][j] != m_vecTileMap_Buffer[i][j])
			{
				Print_Tile(j + iOffset, i + iOffset, m_vecTileMap[i][j]);
				m_vecTileMap_Buffer[i][j] = m_vecTileMap[i][j];
			}
		}
		cout << endl;
	}
	GotoXY(0, 0);
}

void CMap::Change_Tile(POS _Pos, TILE_TYPE _Type)
{
	m_vecTileMap[_Pos.iY][_Pos.iX] = _Type;

	if (_Type == START)
	{
		m_StartPos = _Pos;
	}

	if (_Type == END)
	{
		m_EndPos = _Pos;
	}
}

void CMap::Change_Tile_Check(POS _Pos)
{
	m_vecTileMap[_Pos.iY][_Pos.iX] = m_CHECK_Color;
}

TILE_TYPE CMap::Get_Tile_Type(POS _Pos)
{
	return m_vecTileMap[_Pos.iY][_Pos.iX];
}

void CMap::Change_CHECK_Color()
{
	if (m_CHECK_Color == CHECK2)
	{
		m_CHECK_Color = CHECK;
	}
	else
	{
		m_CHECK_Color = CHECK2;
	}
}
