#include "pch.h"
#include "CMainGame.h"

CMainGame::CMainGame()
	: m_hDC(nullptr), m_pPlayer(nullptr), m_dwTime(GetTickCount64()), m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	if (!m_pPlayer)
	{
		m_pPlayer = new CPlayer;
		m_pPlayer->Initialize();
	}

	dynamic_cast<CPlayer*>(m_pPlayer)->Set_Bullet(&m_BulletList);
}

void CMainGame::Update()
{
	m_pPlayer->Update();

	for (list<CObj*>::iterator Iter = m_BulletList.begin(); Iter != m_BulletList.end();)
	{
		if (WINCY * 0.1f > (*Iter)->Get_Info().fY)
		{
			Iter = m_BulletList.erase(Iter);
			continue;
		}

		(*Iter)->Update();
		++Iter;
	}
}

void CMainGame::Render()
{
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount64())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount64();
	}


	Rectangle(m_hDC, WINCX * 0.1f, WINCY * 0.1f, WINCX * 0.9f, WINCY * 0.9f);

	m_pPlayer->Render(m_hDC);

	for (auto& pBullet : m_BulletList)
		pBullet->Render(m_hDC);

	////////////////////////////////////////////////////

	//TCHAR		szBuf[32] = L"";
	// wsprintf(szBuf, L"Bullet : %d", m_BulletList.size());
	// wsprintf(szBuf, L"Bullet : %f", 3.141592f);	// 소수점 자릿수 출력이 불가능

	// swprintf_s(szBuf, L"Bullet : %f", 3.141592f);	// 모든 서식 문자 지원 함수
	// TextOut(m_hDC, 50, 50, szBuf, lstrlen(szBuf));
	
	// RECT	rcRect{ 100, 100, 200, 200 };	
	// DrawText(m_hDC, szBuf, lstrlen(szBuf), &rcRect, DT_CENTER);


}

void CMainGame::Release()
{
	Safe_Delete<CObj*>(m_pPlayer);

	for_each(m_BulletList.begin(), m_BulletList.end(), Safe_Delete<CObj*>);
	m_BulletList.clear();

	ReleaseDC(g_hWnd, m_hDC);
}
