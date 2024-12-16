#include "pch.h"
#include "TileMgr.h"
#include "AbstractFactory.h"
#include "ScrollMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
	:m_TileMap({ nullptr })
{
	
}

CTileMgr::~CTileMgr()
{
	Release();
}

bool CTileMgr::IsValidTile(const POS _tPos)
{
	if (TILECX * TILEX < _tPos.fX || 0.f >_tPos.fX || TILECY * TILEY < _tPos.fY || 0.f > _tPos.fY)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CTileMgr::Initialize()
{
	for (int iRow = 0; iRow < TILEY; ++iRow)
	{
		for (int iCol = 0; iCol < TILEX; ++iCol)
		{
			float fX = (float)(iCol * TILECX) + (TILECX >> 1);
			float fY = (float)(iRow * TILECY) + (TILECY >> 1);

			CObj* pTile = CAbstractFactory<CTile>::Create(fX, fY);
			m_TileMap[iRow][iCol] = pTile;
		}
	}
}

int CTileMgr::Update()
{
	for (int iRow = 0; iRow < TILEY; ++iRow) 
	{
		for (int iCol = 0; iCol < TILEX; ++iCol) 
		{
			m_TileMap[iRow][iCol]->Update();
		}
	}

	return 0;
}

void CTileMgr::Late_Update()
{
	for (int iRow = 0; iRow < TILEY; ++iRow)
	{
		for (int iCol = 0; iCol < TILEX; ++iCol)
		{
			m_TileMap[iRow][iCol]->Late_Update();
		}
	}
}

void CTileMgr::Render(HDC hDC)
{
	 /*for (auto& pTile : m_arrTile)
	 	pTile->Render(hDC);*/

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX;
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY;

	int		iMaxX = (WINCX / TILECX) + CULLIG_OFFSET;
	int		iMaxY = (WINCY / TILECY) + CULLIG_OFFSET;

	int		iMinX = iScrollX;
	int		iMinY = iScrollY;

	//스크롤 값이 양수가 되었다는건 0번부터~ 최대까지 출력해야한다는것
	if (iMinX > 0)
	{
		iMinX = 0;
		iMaxX = (WINCX / TILECX) + CULLIG_OFFSET;
	}
	//음수면 스크롤 ex) 
	else
	{
		iMinX = abs(iMinX);
		iMaxX = iMaxX - iScrollX;
	}

	if (iMinY > 0)
	{
		iMinY = 0;
		iMaxY = (WINCY / TILECY) + CULLIG_OFFSET;
	}
	else
	{
		iMinY = abs(iMinY);
		iMaxY = iMaxY - iScrollY;
	}


	for (int i = iMinY; i < iMaxY; ++i)
	{
		for (int j = iMinX; j < iMaxX; ++j)
		{
			if (0 > i || TILEY <= i || 0 > j || TILEX <= j)
			{
				continue;
			}

			m_TileMap[i][j]->Render(hDC);
		}
	}
}

void CTileMgr::Release()
{
	for_each(m_TileMap.begin(), m_TileMap.end(), [](auto& row) 
		{
			for_each(row.begin(), row.end(), Safe_Delete<CObj*>);
		});
}

void CTileMgr::Set_TileOption(POS _tPos, TILEOPT _eOpt)
{
	int iX = int(_tPos.fX / TILECX);
	int iY = int(_tPos.fY / TILECY);

	if (0 > iY || TILEY <= iY || 0 > iX || TILEX <= iX)
	{
		return;
	}

	static_cast<CTile*>(m_TileMap[iY][iX])->Set_Option(_eOpt);
}

void CTileMgr::Set_TileOption(float _fX, float _fY, TILEOPT _eOpt)
{
	int iX = int(_fX / TILECX);
	int iY = int(_fY / TILECY);

	if (0 > iY || TILEY <= iY || 0 > iX || TILEX <= iX)
	{
		return;
	}

	static_cast<CTile*>(m_TileMap[iY][iX])->Set_Option(_eOpt);
}

void CTileMgr::Set_TileObj(POS _tPos, CObj* _pObj)
{
	int iX = int(_tPos.fX / TILECX);
	int iY = int(_tPos.fY / TILECY);

	if (0 > iY || TILEY <= iY || 0 > iX || TILEX <= iX)
	{
		return;
	}

	static_cast<CTile*>(m_TileMap[iY][iX])->Set_Obj(_pObj);
}

TILEOPT CTileMgr::Get_TileOption(POS _tPos)
{
	int iX = int(_tPos.fX / TILECX);
	int iY = int(_tPos.fY / TILECY);

	if (0 > iY || TILEY <= iY || 0 > iX || TILEX <= iX)
	{
		return  OPT_END;
	}

	return static_cast<CTile*>(m_TileMap[iY][iX])->Get_Option();
}

TILEOPT CTileMgr::Get_TileOption(float _fX, float _fY)
{
	int iX = int(_fX / TILECX);
	int iY = int(_fY / TILECY);

	if (0 > iY || TILEY <= iY || 0 > iX || TILEX <= iX)
	{
		return  OPT_END;
	}

	return static_cast<CTile*>(m_TileMap[iY][iX])->Get_Option();
}

TILEOPT CTileMgr::Get_TileOption(int _iIndexX, int _iIndexY)
{
	if (0 > _iIndexY || TILEY <= _iIndexY || 0 > _iIndexX || TILEX <= _iIndexX)
	{
		return  OPT_END;
	}

	return static_cast<CTile*>(m_TileMap[_iIndexY][_iIndexX])->Get_Option();
}

CObj* CTileMgr::Get_TileObj(POS _tPos)
{
	int iX = int(_tPos.fX / TILECX);
	int iY = int(_tPos.fY / TILECY);

	if (0 > iY || TILEY <= iY || 0 > iX || TILEX <= iX)
	{
		return  nullptr;
	}
		
	CObj* pObj = static_cast<CTile*>(m_TileMap[iY][iX])->Get_Obj();
	return pObj;
}

CObj* CTileMgr::Get_TileObj(int _iIndexX, int _iIndexY)
{
	if (0 > _iIndexY || TILEY <= _iIndexY || 0 > _iIndexX || TILEX <= _iIndexX)
	{
		return  nullptr;
	}

	CObj* pObj = static_cast<CTile*>(m_TileMap[_iIndexY][_iIndexX])->Get_Obj();
	return pObj;
}

CObj* CTileMgr::Get_TileObj(float _fX, float _fY)
{
	int iX = int(_fX / TILECX);
	int iY = int(_fY / TILECY);

	if (0 > iY || TILEY <= iY || 0 > iX || TILEX <= iX)
	{
		return  nullptr;
	}

	CObj* pObj = static_cast<CTile*>(m_TileMap[iY][iX])->Get_Obj();
	return pObj;
}

int CTileMgr::Get_TileIndex(float _fX, float _fY)
{
	return int(_fX / TILECX) + int(_fY / TILECY) * TILEX;
}

CTile* CTileMgr::Find_ReachableTiles(float _fX, float _fY)
{
	int iCheckIndex(0);//CTileMgr::Get_TileIndex(_fX, _fY);

	return nullptr;
}

void CTileMgr::Picking_Tile(POINT pt, int iDrawID, int iOption)
{
	/*int		x = pt.x / TILECX;
	int		y = pt.y / TILECY;

	int	iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_arrTile.size())
		return;*/

	/*dynamic_cast<CTile*>(m_arrTile[iIndex])->Set_DrawID(iDrawID);
	dynamic_cast<CTile*>(m_arrTile[iIndex])->Set_Option(iOption);*/

}
