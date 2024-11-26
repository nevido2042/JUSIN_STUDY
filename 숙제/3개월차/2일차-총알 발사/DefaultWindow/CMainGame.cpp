#include "pch.h"
#include "CMainGame.h"
#include "Monster.h"
#include "Mario.h"

CMainGame::CMainGame()
	: m_hDC(nullptr), m_pPlayer(nullptr), m_dwTime(GetTickCount64()), m_iFPS(0),
	m_pMario(nullptr), m_pGoomba(nullptr)
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

	if (!m_pMario)
	{
		m_pMario = new CMario;
		m_pMario->Initialize();
	}

	if (!m_pGoomba)
	{
		m_pGoomba = new CMonster();
		m_pGoomba->Initialize();
		m_pGoomba->Set_Pos(MINI_WINCX_RIGHT - MINI_WINCX_LEFT, MINI_WINCY_BOTTOM);
		m_pGoomba->Set_Speed(1.f);
	}

	//if (!m_pMonster)
	//{
	//	m_pMonster = new CMonster;
	//	m_pMonster->Initialize();
	//}

	for (int i = 1; i < 10; ++i)
	{
		CMonster* pMonster = new CMonster;
		pMonster->Initialize();
		pMonster->Set_Pos(WINCX * 0.1f * i, WINCY * 0.5f);
		m_MonsterList.push_back(dynamic_cast<CObj*>(pMonster));
	}
	for (int i = 1; i < 10; ++i)
	{
		CMonster* pMonster = new CMonster;
		pMonster->Initialize();
		pMonster->Set_Pos(WINCX * 0.1f * i, WINCY * 0.2f);
		m_MonsterList.push_back(dynamic_cast<CObj*>(pMonster));
	}

	dynamic_cast<CPlayer*>(m_pPlayer)->Set_Bullet(&m_BulletList);
}

void CMainGame::Update()
{
	m_pPlayer->Update();
	m_pMario->Update();
	m_pGoomba->Update();
	/*if (m_pMonster)
	{
		m_pMonster->Update();
	}*/
	if (m_MonsterList.size() == 0)
	{
		PostQuitMessage(0);
	}

	for (list<CObj*>::iterator Iter = m_MonsterList.begin(); Iter != m_MonsterList.end();)
	{
		(*Iter)->Update();

		if (CMonster* pMonster = dynamic_cast<CMonster*>((*Iter)))
		{
			if (pMonster->Get_IsDead())
			{
				Safe_Delete(pMonster);
				Iter = m_MonsterList.erase(Iter);
				continue;
			}
		}

		++Iter;
	}




	RECT rc;
	//마리오 굼바 충돌 검사
	if (IntersectRect(&rc, &(m_pMario)->Get_Rect(), &(m_pGoomba)->Get_Rect()))
	{
		PostQuitMessage(0);
	}
	//범위 벗어날 시 총알 삭wa제
	for (list<CObj*>::iterator BulletIter = m_BulletList.begin(); BulletIter != m_BulletList.end();)
	{
		if (MINI_WINCY_TOP > (*BulletIter)->Get_Info().fY||
			MINI_WINCX_LEFT > (*BulletIter)->Get_Info().fX||
			MINI_WINCY_BOTTOM < (*BulletIter)->Get_Info().fY||
			MINI_WINCX_RIGHT < (*BulletIter)->Get_Info().fX)
		{
			Safe_Delete(*BulletIter);
			BulletIter = m_BulletList.erase(BulletIter);
			continue;
		}



		//몬스터, 총알 충돌 검사
		for (list<CObj*>::iterator MonsterIter = m_MonsterList.begin(); MonsterIter != m_MonsterList.end();)
		{
			if (BulletIter == m_BulletList.end())
			{
				break;
			}

			if (IntersectRect(&rc, &(*MonsterIter)->Get_Rect(), &(*BulletIter)->Get_Rect()))
			{
				if (CMonster* pMonster = dynamic_cast<CMonster*>(*MonsterIter))
				{
					pMonster->Add_HP(-1);
				}
				//몬스터 충돌 시 총알 삭제
				Safe_Delete(*BulletIter);
				BulletIter = m_BulletList.erase(BulletIter);
				continue;
			}
			++MonsterIter;
		}

		if (BulletIter == m_BulletList.end())
		{
			break;
		}

		(*BulletIter)->Update();
		++BulletIter;
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


	Rectangle(m_hDC, int(MINI_WINCX_LEFT), int(MINI_WINCY_TOP)
		, int(MINI_WINCX_RIGHT), int(MINI_WINCY_BOTTOM));

	m_pPlayer->Render(m_hDC);
	m_pMario->Render(m_hDC);
	m_pGoomba->Render(m_hDC);

	for (list<CObj*>::iterator Iter = m_MonsterList.begin(); Iter != m_MonsterList.end();)
	{
		if (*Iter)
		{
			(*Iter)->Render(m_hDC);
		}
		++Iter;
	}

	for (auto& pBullet : m_BulletList)
		pBullet->Render(m_hDC);

	////////////////////////////////////////////////////

	TCHAR		szBuf[32] = L"";
	//wsprintf(szBuf, L"Bullet : %I64u", m_BulletList.size());
	// wsprintf(szBuf, L"Bullet : %f", 3.141592f);	// 소수점 자릿수 출력이 불가능

	swprintf_s(szBuf, L"Bullet : %I64u", m_BulletList.size());	// 모든 서식 문자 지원 함수
	TextOut(m_hDC, 10, 10, szBuf, lstrlen(szBuf));
	
	// RECT	rcRect{ 100, 100, 200, 200 };	
	// DrawText(m_hDC, szBuf, lstrlen(szBuf), &rcRect, DT_CENTER);
}

void CMainGame::Release()
{
	Safe_Delete<CObj*>(m_pPlayer);
	Safe_Delete<CObj*>(m_pMario);
	Safe_Delete<CObj*>(m_pGoomba);

	//Safe_Delete<CObj*>(m_pMonster);
	for_each(m_MonsterList.begin(), m_MonsterList.end(), Safe_Delete<CObj*>);
	m_MonsterList.clear();

	for_each(m_BulletList.begin(), m_BulletList.end(), Safe_Delete<CObj*>);
	m_BulletList.clear();

	ReleaseDC(g_hWnd, m_hDC);
}
