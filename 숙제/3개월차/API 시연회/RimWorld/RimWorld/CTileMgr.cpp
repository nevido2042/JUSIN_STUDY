#include "pch.h"
#include "CTileMgr.h"
#include "CAbstractFactory.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
	m_vecTile.reserve(TILEX * TILEY);
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
			m_vecTile.push_back(pTile);
		}
	}
}

int CTileMgr::Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Update();

	return 0;
}

void CTileMgr::Late_Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Late_Update();
}

void CTileMgr::Render(HDC hDC)
{
	for (auto& pTile : m_vecTile)
		pTile->Render(hDC);
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CObj*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTileMgr::Picking_Tile(POINT pt, int iDrawID, int iOption)
{
	int		x = pt.x / TILECX;
	int		y = pt.y / TILECY;

	int	iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawID(iDrawID);
	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_Option(iOption);

}
