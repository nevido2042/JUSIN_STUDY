#include "pch.h"
#include "TileMgr.h"
#include "AbstractFactory.h"
#include "ScrollMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
	:m_arrTile{nullptr}
{
	
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (float)(j * TILECX) + (TILECX >> 1);
			float fY = (float)(i * TILECY) + (TILECY >> 1);

			CObj* pTile = CAbstractFactory<CTile>::Create(fX, fY);
			m_arrTile[i * TILEX + j] = pTile;
		}
	}
}

int CTileMgr::Update()
{
	for (auto& pTile : m_arrTile)
		pTile->Update();

	return 0;
}

void CTileMgr::Late_Update()
{
	for (auto& pTile : m_arrTile)
		pTile->Late_Update();
}

void CTileMgr::Render(HDC hDC)
{
	 /*for (auto& pTile : m_arrTile)
	 	pTile->Render(hDC);*/

	int		iScrollX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	int		iScrollY = abs((int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY);

	int		iMaxX = iScrollX + WINCX / TILECX + 2;
	int		iMaxY = iScrollY + WINCY / TILECY + 2;

	for (int i = iScrollY; i < iMaxY; ++i)
	{
		for (int j = iScrollX; j < iMaxX; ++j)
		{
			int		iIndex = i * TILEX + j;

			if (0 > iIndex || m_arrTile.size() <= (size_t)iIndex)
				continue;

			m_arrTile[iIndex]->Render(hDC);
		}
	}
}

void CTileMgr::Release()
{
	for_each(m_arrTile.begin(), m_arrTile.end(), Safe_Delete<CObj*>);
}

void CTileMgr::Picking_Tile(POINT pt, int iDrawID, int iOption)
{
	int		x = pt.x / TILECX;
	int		y = pt.y / TILECY;

	int	iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_arrTile.size())
		return;

	/*dynamic_cast<CTile*>(m_arrTile[iIndex])->Set_DrawID(iDrawID);
	dynamic_cast<CTile*>(m_arrTile[iIndex])->Set_Option(iOption);*/

}
