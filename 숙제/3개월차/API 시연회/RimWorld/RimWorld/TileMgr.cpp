#include "pch.h"
#include "TileMgr.h"
#include "AbstractFactory.h"
#include "ScrollMgr.h"
#include "PathFinder.h"
#include "BmpMgr.h"
#include "Camera.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
	:m_TileMap({ nullptr }),
	m_iMinX(0), m_iMaxX(0), m_iMinY(0), m_iMaxY(0)
{
	
}

CTileMgr::~CTileMgr()
{
	Release();
}

bool CTileMgr::IsValidTile(const POS _tPos)
{
	if (TILECX * TILEX < _tPos.iX || 0.f >_tPos.iX || TILECY * TILEY < _tPos.iY || 0.f > _tPos.iY)
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
	/*for (int iRow = 0; iRow < TILEY; ++iRow) 
	{
		for (int iCol = 0; iCol < TILEX; ++iCol) 
		{
			m_TileMap[iRow][iCol]->Update();
		}
	}*/

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX;
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY;

	m_iMaxX = (WINCX / TILECX) + CULLIG_OFFSET;
	m_iMaxY = (WINCY / TILECY) + CULLIG_OFFSET;

	m_iMinX = iScrollX;
	m_iMinY = iScrollY;

	//스크롤 값이 양수가 되었다는건 0번부터~ 최대까지 출력해야한다는것
	if (m_iMinX > 0)
	{
		m_iMinX = 0;
		m_iMaxX = (WINCX / TILECX) + CULLIG_OFFSET;
	}
	//음수면 스크롤 ex) 
	else
	{
		m_iMinX = abs(m_iMinX);
		m_iMaxX = m_iMaxX - iScrollX;
	}

	if (m_iMinY > 0)
	{
		m_iMinY = 0;
		m_iMaxY = (WINCY / TILECY) + CULLIG_OFFSET;
	}
	else
	{
		m_iMinY = abs(m_iMinY);
		m_iMaxY = m_iMaxY - iScrollY;
	}


	for (int i = m_iMinY; i < m_iMaxY; ++i)
	{
		for (int j = m_iMinX; j < m_iMaxX; ++j)
		{
			if (0 > i || TILEY <= i || 0 > j || TILEX <= j)
			{
				continue;
			}

			m_TileMap[i][j]->Update();
		}
	}


	return 0;
}

void CTileMgr::Late_Update()
{
	/*for (int iRow = 0; iRow < TILEY; ++iRow)
	{
		for (int iCol = 0; iCol < TILEX; ++iCol)
		{
			m_TileMap[iRow][iCol]->Late_Update();
		}
	}*/

	for (int i = m_iMinY; i < m_iMaxY; ++i)
	{
		for (int j = m_iMinX; j < m_iMaxX; ++j)
		{
			if (0 > i || TILEY <= i || 0 > j || TILEX <= j)
			{
				continue;
			}

			m_TileMap[i][j]->Late_Update();
		}
	}
}

void CTileMgr::Render(HDC hDC)
{
	// /*for (auto& pTile : m_arrTile)
	// 	pTile->Render(hDC);*/

	//int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();// / TILECX;
	//int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();// / TILECY;

	//HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Gravel");

	//BitBlt(hDC,
	//	iScrollX, iScrollY,
	//	int(TILEX * TILECX), int(TILEY * TILECY),
	//	hMemDC, 0, 0, SRCCOPY);


	////int		iMaxX = (WINCX / TILECX) + CULLIG_OFFSET;
	////int		iMaxY = (WINCY / TILECY) + CULLIG_OFFSET;

	////int		iMinX = iScrollX;
	////int		iMinY = iScrollY;

	//////스크롤 값이 양수가 되었다는건 0번부터~ 최대까지 출력해야한다는것
	////if (iMinX > 0)
	////{
	////	iMinX = 0;
	////	iMaxX = (WINCX / TILECX) + CULLIG_OFFSET;
	////}
	//////음수면 스크롤 ex) 
	////else
	////{
	////	iMinX = abs(iMinX);
	////	iMaxX = iMaxX - iScrollX;
	////}

	////if (iMinY > 0)
	////{
	////	iMinY = 0;
	////	iMaxY = (WINCY / TILECY) + CULLIG_OFFSET;
	////}
	////else
	////{
	////	iMinY = abs(iMinY);
	////	iMaxY = iMaxY - iScrollY;
	////}


	//for (int i = m_iMinY; i < m_iMaxY; ++i)
	//{
	//	for (int j = m_iMinX; j < m_iMaxX; ++j)
	//	{
	//		if (0 > i || TILEY <= i || 0 > j || TILEX <= j)
	//		{
	//			continue;
	//		}

	//		m_TileMap[i][j]->Render(hDC);
	//	}
	//}

	for (int i = 0; i < TILECY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			if (0 > i || TILEY <= i || 0 > j || TILEX <= j)
			{
				continue;
			}

			CObj* pTile = m_TileMap[i][j];

			if (CCamera::Get_Instance()->IsInCameraView(pTile->Get_Info().fX, pTile->Get_Info().fY, pTile->Get_Info().fCX, pTile->Get_Info().fCY))
			{
				pTile->Render(hDC);
			}


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
	int iX = int(_tPos.iX / TILECX);
	int iY = int(_tPos.iY / TILECY);

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
	int iX = int(_tPos.iX / TILECX);
	int iY = int(_tPos.iY / TILECY);

	if (0 > iY || TILEY <= iY || 0 > iX || TILEX <= iX)
	{
		return;
	}

	static_cast<CTile*>(m_TileMap[iY][iX])->Set_Obj(_pObj);
}

void CTileMgr::Set_TileObj(float _fX, float _fY, CObj* _pObj)
{
	int iX = int(_fX / TILECX);
	int iY = int(_fY / TILECY);

	if (0 > iY || TILEY <= iY || 0 > iX || TILEX <= iX)
	{
		return;
	}

	static_cast<CTile*>(m_TileMap[iY][iX])->Set_Obj(_pObj);
}

TILEOPT CTileMgr::Get_TileOption(POS _tPos)
{
	int iX = int(_tPos.iX / TILECX);
	int iY = int(_tPos.iY / TILECY);

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
	int iX = int(_tPos.iX / TILECX);
	int iY = int(_tPos.iY / TILECY);

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

list<CNode*> CTileMgr::Find_ReachableTiles(POS _tStart, POS _tEnd)
{
	int iX = int(_tEnd.iX / TILECX);
	int iY = int(_tEnd.iY / TILECY);

	//8방향
	const int arrDir[8][2] = 
	{
		{0,	-1},
		{1,	-1},
		{1,	 0}, 
		{1,	 1}, 
		{0,	 1}, 
		{-1, 1},
		{-1, 0},
		{-1,-1}
	};

	list<CNode*> pNodeList;

	for (int i = 0; i < 8; ++i)
	{
		if (CTileMgr::Get_TileOption(iX + arrDir[i][0], iY + arrDir[i][1]) == OPT_REACHABLE)
		{
			//그 타일의 길을 찾을 수 있는가?
			pNodeList = CPathFinder::Get_Instance()
				->Find_Path(_tStart, POS{ (iX + arrDir[i][0]) * TILECX + (int)(TILECX * 0.5f) , (iY + arrDir[i][1]) * TILECY + (int)(TILECY * 0.5f) });

			if (pNodeList.empty())
			{
				continue;
			}

			return pNodeList;
		}
	}
	
	return pNodeList;
}

CObj* CTileMgr::Get_Tile(int _iX, int _iY)
{
	return m_TileMap[_iY][_iX];
}

CObj* CTileMgr::Get_Tile(float _fX, float _fY)
{
	int iX = int(_fX / TILECX);
	int iY = int(_fY / TILECY);

	return m_TileMap[iY][iX];
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
