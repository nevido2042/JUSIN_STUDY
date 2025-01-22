#include "pch.h"
#include "CTerrain.h"
#include "CTextureMgr.h"
#include "CDevice.h"

CTerrain::CTerrain()
{
	m_vecTileMap.reserve(TILEX * TILEY);
}

CTerrain::~CTerrain()
{
	Release();
}

HRESULT CTerrain::Initialize()
{
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Stage/Terrain/Tile/Tile%d.png",
		TEX_MULTI, L"Terrain", L"Tile", 36)))
	{
		AfxMessageBox(L"Terrain Texture Insert Failed");
		return E_FAIL;
	}

	m_vecTileMap.resize(TILEY);
	//for (int i = 0; i < TILEY; ++i)
	//{
	//	m_vecTileMap.at(i).reserve(TILEX);
	//}

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE* pTile = new TILE;

			float	fY = (TILECY / 2.f) * i;
			float	fX = (TILECX * j) + (i % 2) * (TILECX / 2.f);

			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { (float)TILECX, (float)TILECY };
			pTile->byOption = 0;
			pTile->byDrawID = 3;

			m_vecTileMap[i].push_back(pTile);
		}
	}

	return S_OK;
}

void CTerrain::Update()
{
	
}

void CTerrain::Render()
{
	D3DXMATRIX	matWorld, matScale, matTrans;

	TCHAR	szBuf[MIN_STR] = L"";
	int		iIndex(0);

	for (int i = 0; i < TILEY; ++i)
	{
		for (auto pTile : m_vecTileMap[i])
		{
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			D3DXMatrixTranslation(&matTrans, pTile->vPos.x, pTile->vPos.y, pTile->vPos.z);

			matWorld = matScale * matTrans;

			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

			const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", pTile->byDrawID);

			float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
			float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

			D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };

			CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //����� �ؽ�ó �İ�ü
				nullptr,		// ����� �̹��� ������ ���� Rect �ּ�, null�� ��� �̹����� 0, 0�������� ���
				&vTemp,		// ����� �̹����� �߽� ��ǥ vec3 �ּ�, null�� ��� 0, 0 �̹��� �߽�
				nullptr,		// ��ġ ��ǥ�� ���� vec3 �ּ�, null�� ��� ��ũ�� �� 0, 0 ��ǥ ���	
				D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� �̹����� ���� ���� ��, 0xffffffff�� �Ѱ��ָ� ���� �ʰ� ���� ���� ����

			swprintf_s(szBuf, L"%d", iIndex);

			CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
				szBuf,		// ����� ���ڿ�
				lstrlen(szBuf),  // ���ڿ� ������ ũ��
				nullptr,	// ����� ��Ʈ ��ġ
				0,			// ���� ����(�ɼ�)
				D3DCOLOR_ARGB(255, 255, 255, 255));

			iIndex++;
		}
	}
	
}

void CTerrain::Release()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			Safe_Delete(m_vecTileMap[i][j]);
		}
	}
	//for_each(m_vecTileMap.begin(), m_vecTileMap.end(), [](auto& p) 
	//	{ 
	//		if (p)
	//		{ delete p; p = nullptr; }
	//	});

	m_vecTileMap.clear();
	m_vecTileMap.shrink_to_fit();
}

void CTerrain::Tile_Change(const D3DXVECTOR3& vPos, const BYTE& byDrawID)
{
	//int iIndex = Get_TileIdx(vPos);

	//if (-1 == iIndex)
	//	return;

	//m_vecTileMap[iIndex]->byDrawID = byDrawID;
	//m_vecTileMap[iIndex]->byOption = 1;

	int iX(0), iY(0);

	if (Get_TileIdx(vPos, iX, iY))
	{
		m_vecTileMap[iY][iX]->byDrawID = byDrawID;
		m_vecTileMap[iY][iX]->byOption = 1;
	}
	
}


//int CTerrain::Get_TileIdx(const D3DXVECTOR3& vPos)
//{
//	for (size_t index = 0; index < m_vecTileMap.size(); ++index)
//	{
//		if (Picking_Dot(vPos, index))
//		{
//			return index;
//		}
//	}
//
//	return -1;
//}

bool CTerrain::Get_TileIdx(const D3DXVECTOR3& vPos, int& _iOutX, int& _iOutY)
{
	float _fX(vPos.x);
	float _fY(vPos.y);

	int iX(-1), iY(-1);
	D3DXVECTOR3 tPointA, tPointB;
	float fSlope(0.f);
	float fIntercept(0.f);

#pragma region ���� ��ǥ �ε��� ���
	// ���� ���� ��踦 Ȯ��: ���� ���� �������� �ۿ� ������ �� ����
	tPointA = m_vecTileMap[0][TILEX - 1]->vPos;
	tPointA.x += TILECX * 0.5f;
	tPointB = m_vecTileMap[TILEY - 1][TILEX - 1]->vPos;
	tPointB.x += TILECX * 0.5f;

	fSlope = (tPointA.y - tPointB.y) / (tPointA.x - tPointB.x); // ����
	fIntercept = tPointA.y - fSlope * tPointA.x;              // y-����

	// ���� ��� �ۿ� ������ �� ����
	if (fSlope * _fX + fIntercept < _fY)
	{
		return false;
	}

	// ���� ������ �������� Ȯ��
	for (int j = 0; j < TILEX; ++j)
	{
		tPointA = m_vecTileMap[0][j]->vPos;
		tPointA.x -= TILECX * 0.5f;
		tPointB = m_vecTileMap[TILEY - 1][j]->vPos;
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
	tPointA = m_vecTileMap[TILEY - 1][0]->vPos;
	tPointA.x -= TILECX * 0.5f;
	tPointB = m_vecTileMap[TILEY - 1][TILEX - 1]->vPos;
	tPointB.x -= TILECX * 0.5f;

	fSlope = (tPointA.y - tPointB.y) / (tPointA.x - tPointB.x); // ����
	fIntercept = tPointA.y - fSlope * tPointA.x;              // y-����

	// ���� ��� �ۿ� ������ �� ����
	if (fSlope * _fX + fIntercept < _fY)
	{
		return false;
	}

	// ��� ������ �������� Ȯ��
	for (int i = 0; i < TILEY; ++i)
	{
		tPointA = m_vecTileMap[i][0]->vPos;
		tPointA.x += TILECX * 0.5f;
		tPointB = m_vecTileMap[i][TILEX - 1]->vPos;
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
		return false;
	}

	_iOutX = iX;
	_iOutY = iY;

	return true;
}

//bool CTerrain::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
//{
//	// 12 -> 3 -> 6 -> 9
//	float	fGradient[4] =
//	{
//		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
//		(TILECY / 2.f) / (TILECX / 2.f),
//		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
//		(TILECY / 2.f) / (TILECX / 2.f)
//	};
//
//	// 12 -> 3 -> 6 -> 9
//	D3DXVECTOR3	vPoint[4] =
//	{
//		{ m_vecTileMap[iIndex]->vPos.x, m_vecTileMap[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
//		{ m_vecTileMap[iIndex]->vPos.x + (TILECX / 2.f), m_vecTileMap[iIndex]->vPos.y, 0.f },
//		{ m_vecTileMap[iIndex]->vPos.x, m_vecTileMap[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
//		{ m_vecTileMap[iIndex]->vPos.x - (TILECX / 2.f), m_vecTileMap[iIndex]->vPos.y, 0.f },
//
//	};
//
//	// y = ax + b;
//	// -b = ax - y
//	// b = y - ax
//
//	float	fB[4] =
//	{
//		vPoint[0].y - fGradient[0] * vPoint[0].x,
//		vPoint[1].y - fGradient[1] * vPoint[1].x,
//		vPoint[2].y - fGradient[2] * vPoint[2].x,
//		vPoint[3].y - fGradient[3] * vPoint[3].x
//	};
//
//	// 0 == ax + b - y // �� �� ����
//	// 0 > ax + b - y // ���� ����
//	// 0 < ax + b - y // ���� �Ʒ���
//
//	bool	bCheck[4]{ false };
//
//	// 12 -> 3
//	if (0 < fGradient[0] * vPos.x + fB[0] - vPos.y)
//		bCheck[0] = true;
//
//	// 3 -> 6
//	if (0 >= fGradient[1] * vPos.x + fB[1] - vPos.y)
//		bCheck[1] = true;
//
//	// 6 -> 9
//	if (0 >= fGradient[2] * vPos.x + fB[2] - vPos.y)
//		bCheck[2] = true;
//
//	// 9 -> 12
//	if (0 < fGradient[3] * vPos.x + fB[3] - vPos.y)
//		bCheck[3] = true;
//
//
//	return bCheck[0] && bCheck[1] && bCheck[2] && bCheck[3];
//}

//bool CTerrain::Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex)
//{
//	// 12 -> 3 -> 6 -> 9
//	D3DXVECTOR3	vPoint[4] =
//	{
//		{ m_vecTileMap[iIndex]->vPos.x, m_vecTileMap[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
//		{ m_vecTileMap[iIndex]->vPos.x + (TILECX / 2.f), m_vecTileMap[iIndex]->vPos.y, 0.f },
//		{ m_vecTileMap[iIndex]->vPos.x, m_vecTileMap[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
//		{ m_vecTileMap[iIndex]->vPos.x - (TILECX / 2.f), m_vecTileMap[iIndex]->vPos.y, 0.f },
//
//	};
//
//	D3DXVECTOR3	vDir[4] =
//	{
//		vPoint[1] - vPoint[0],
//		vPoint[2] - vPoint[1],
//		vPoint[3] - vPoint[2],
//		vPoint[0] - vPoint[3]
//	};
//
//	D3DXVECTOR3		vNormal[4] =
//	{
//		{ -vDir[0].y , vDir[0].x, 0.f },
//		{ -vDir[1].y , vDir[1].x, 0.f },
//		{ -vDir[2].y , vDir[2].x, 0.f },
//		{ -vDir[3].y , vDir[3].x, 0.f },
//	};
//
//	D3DXVECTOR3		vMouseDir[4] =
//	{
//		vPos - vPoint[0],
//		vPos - vPoint[1],
//		vPos - vPoint[2],
//		vPos - vPoint[3]
//	};
//
//	for (int i = 0; i < 4; ++i)
//	{
//		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
//		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
//	}
//
//	for (int i = 0; i < 4; ++i)
//	{
//		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
//			return false;
//	}
//
//	return true;
//}
