#include "pch.h"
#include "CTileMgr.h"
#include "CTextureMgr.h"

IMPLEMENT_SINGLETON(CTileMgr)

void CTileMgr::Change_Tile(float _fX, float _fY)
{
	int iX(-1), iY(0);

	//가장 우측의 직선보다 밖에 있으면 맵밖임
	D3DXVECTOR3 tPointA = m_vecTileMap[0][TILEX-1]->Get_Info().vPos;
	tPointA.x += (float)TILECX * 0.5f;
	D3DXVECTOR3 tPointB = m_vecTileMap[TILEY - 1][TILEX-1]->Get_Info().vPos;
	tPointB.x += (float)TILECX * 0.5f;
	float fSlope = (tPointA.y - tPointB.y) / (tPointA.x - tPointB.x);	//기울기
	float fYIntercept = tPointA.y - fSlope * tPointA.x;					// y-절편
	if (fSlope * _fX + fYIntercept < _fY) //마우스가 맵 밖에 있다.
	{
		return;
	}

	for (int j = 0; j < TILEX; ++j)
	{
		//좌측 직선의 방적식.
		D3DXVECTOR3 tPointA = m_vecTileMap[0][j]->Get_Info().vPos;
		tPointA.x -= (float)TILECX * 0.5f;
		D3DXVECTOR3 tPointB = m_vecTileMap[TILEY - 1][j]->Get_Info().vPos;
		tPointB.x -= (float)TILECX * 0.5f;

		float fSlope = (tPointA.y - tPointB.y) / (tPointA.x - tPointB.x);	//기울기
		float fYIntercept = tPointA.y - fSlope * tPointA.x;					// y-절편

		if (fSlope * _fX + fYIntercept > _fY) //마우스가 맵 밖에 있다.
		{
			break;
		}

		if (fSlope * _fX + fYIntercept == _fY || fSlope * _fX + fYIntercept < _fY)
		{
			//점이 선 위에 있다
			//점이 선 우측에있다.
			++iX;
		}
	}
	
	if (iX < 0 || iX >TILEX - 1 || iY < 0 || iY > TILEY - 1)
	{
		return;
	}

	m_vecTileMap[iY][iX]->Set_TexInfo(CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", rand() % 36));
	InvalidateRect(g_hWnd, nullptr, false);
}

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
