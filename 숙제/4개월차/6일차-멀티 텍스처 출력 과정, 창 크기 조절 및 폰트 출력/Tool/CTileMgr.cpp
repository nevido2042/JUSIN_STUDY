#include "pch.h"
#include "CTileMgr.h"
#include "CTextureMgr.h"

IMPLEMENT_SINGLETON(CTileMgr)

void CTileMgr::Initialize()
{
	m_vecTileMap.resize(TILEY);
	for (int i = 0; i < TILEY; ++i)
	{
		m_vecTileMap.at(i).resize(TILEX);
	}

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			CObj* pTile = new CTile;
			pTile->Initialize();
			pTile->Set_Pos(float(j * TILECX * 0.5f) - float(i * TILECX * 0.5f),
				float(j * TILECY * 0.5f) + float(i * TILECY * 0.5f), 0.f);
			m_vecTileMap[i][j] = pTile;
		}
	}
}

void CTileMgr::Render(CDevice* _pDevice)
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			m_vecTileMap[i][j]->Render(_pDevice);



		}
	}
}

void CTileMgr::Release()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			Safe_Delete(m_vecTileMap[i][j]);
		}
	}
}

CTileMgr::CTileMgr()
{
}

CTileMgr::~CTileMgr()
{
	Release();
}
