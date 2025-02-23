#include "pch.h"
#include "CTileMgr.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"

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
	// for (auto& pTile : m_vecTile)
	// 	pTile->Render(hDC);

	int		iScrollX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	int		iScrollY = abs((int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY);

	int		iMaxX = iScrollX + WINCX / TILECX + 2;
	int		iMaxY = iScrollY + WINCY / TILECY + 2;

	for (int i = iScrollY; i < iMaxY; ++i)
	{
		for (int j = iScrollX; j < iMaxX; ++j)
		{
			int		iIndex = i * TILEX + j;

			if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
				continue;

			m_vecTile[iIndex]->Render(hDC);
		}
	}




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

void CTileMgr::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	int		iDrawID(0), iOption(0);
	DWORD	dwByte(0);

	for (auto& pTile : m_vecTile)
	{
		iDrawID = dynamic_cast<CTile*>(pTile)->Get_DrawID();
		iOption = dynamic_cast<CTile*>(pTile)->Get_Option();

		WriteFile(hFile, pTile->Get_Info_Pointer(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &iOption, sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Tile Save", L"성공", MB_OK);

}

void CTileMgr::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	int		iDrawID(0), iOption(0);
	DWORD	dwByte(0);
	INFO	tTile{};

	Release();

	while (true)
	{
		ReadFile(hFile, &tTile, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, NULL);

		if (0 == dwByte)
			break;

		CObj* pTile = CAbstractFactory<CTile>::Create(tTile.fX, tTile.fY);
		dynamic_cast<CTile*>(pTile)->Set_DrawID(iDrawID);
		dynamic_cast<CTile*>(pTile)->Set_Option(iOption);

		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Load Save", L"성공", MB_OK);

}
