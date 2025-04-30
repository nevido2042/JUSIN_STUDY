#include "pch.h"
#include "CTileMgr.h"
#include "CTextureMgr.h"

IMPLEMENT_SINGLETON(CTileMgr)

void CTileMgr::Change_Tile(float _fX, float _fY)
{
	int iX(-1), iY(-1);
	D3DXVECTOR3 tPointA, tPointB;
	float fSlope(0.f);
	float fIntercept(0.f);

#pragma region ���� ��ǥ �ε��� ���
	// ���� ���� ��踦 Ȯ��: ���� ���� �������� �ۿ� ������ �� ����
	tPointA = m_vecTileMap[0][TILEX - 1]->Get_Info().vPos;
	tPointA.x += TILECX * 0.5f;
	tPointB = m_vecTileMap[TILEY - 1][TILEX - 1]->Get_Info().vPos;
	tPointB.x += TILECX * 0.5f;

	fSlope = (tPointA.y - tPointB.y) / (tPointA.x - tPointB.x); // ����
	fIntercept = tPointA.y - fSlope * tPointA.x;              // y-����

	// ���� ��� �ۿ� ������ �� ����
	if (fSlope * _fX + fIntercept < _fY)
	{
		return;
	}

	// ���� ������ �������� Ȯ��
	for (int j = 0; j < TILEX; ++j)
	{
		tPointA = m_vecTileMap[0][j]->Get_Info().vPos;
		tPointA.x -= TILECX * 0.5f;
		tPointB = m_vecTileMap[TILEY - 1][j]->Get_Info().vPos;
		tPointB.x -= TILECX * 0.5f;

		// ����� ���� ���
		fSlope = (tPointA.y - tPointB.y) / (tPointA.x - tPointB.x);  // ����
		fIntercept = tPointA.y - fSlope * tPointA.x;                 // y-����

		// ���콺�� �� �ۿ� �ִ��� Ȯ��
		if (fSlope * _fX + fIntercept > _fY)
		{
			break;
		}

		// ���� �� ���� �ִٸ� X ��ǥ ����
		if (fSlope * _fX + fIntercept <= _fY)
		{
			++iX;
		}
	}
#pragma endregion

#pragma region ���� ��ǥ �ε��� ���
	// ���� �ϴ� ��踦 Ȯ��: ���� �ϴ� �������� �ۿ� ������ �� ����
	tPointA = m_vecTileMap[TILEY - 1][0]->Get_Info().vPos;
	tPointA.x -= TILECX * 0.5f;
	tPointB = m_vecTileMap[TILEY - 1][TILEX - 1]->Get_Info().vPos;
	tPointB.x -= TILECX * 0.5f;

	fSlope = (tPointA.y - tPointB.y) / (tPointA.x - tPointB.x); // ����
	fIntercept = tPointA.y - fSlope * tPointA.x;              // y-����

	// ���� ��� �ۿ� ������ �� ����
	if (fSlope * _fX + fIntercept < _fY)
	{
		return;
	}

	// ��� ������ �������� Ȯ��
	for (int i = 0; i < TILEY; ++i)
	{
		tPointA = m_vecTileMap[i][0]->Get_Info().vPos;
		tPointA.x += TILECX * 0.5f;
		tPointB = m_vecTileMap[i][TILEX - 1]->Get_Info().vPos;
		tPointB.x += TILECX * 0.5f;

		// ����� ���� ���
		fSlope = (tPointA.y - tPointB.y) / (tPointA.x - tPointB.x);  // ����
		fIntercept = tPointA.y - fSlope * tPointA.x;                 // y-����

		// ���콺�� �� �ۿ� �ִ��� Ȯ��
		if (fSlope * _fX + fIntercept > _fY)
		{
			break;
		}

		// ���� �� ���� �ִٸ� X ��ǥ ����
		if (fSlope * _fX + fIntercept <= _fY)
		{
			++iY;
		}
	}
#pragma endregion

	//��ȿ�� �ε����ΰ�?
	if (iX < 0 || iX >TILEX - 1 || iY < 0 || iY > TILEY - 1)
	{
		return;
	}

	//���� ����
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
