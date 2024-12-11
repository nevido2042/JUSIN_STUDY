#include "pch.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "ZoomMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
	:m_arrTile{nullptr}
{

}

CTileMgr::~CTileMgr()
{
}

void CTileMgr::Create_TileMap()
{
	for (int i = 0; i < TILEMAP_SIZE; ++i)
	{
		for (int j = 0; j < TILEMAP_SIZE; ++j)
		{
			CObj* pTile = CAbstractFactory<CTile>::Create((float)j * (float)TILE_SIZE, (float)i * (float)TILE_SIZE);
			m_arrTile[i][j] = pTile;
			CObjMgr::Get_Instance()->Add_Object(OBJ_MOUSE, pTile);
		}
	}	
}
