#include "pch.h"
#include "CMainGame.h"
#include "CAbstractFactory.h"
#include "CCollisionMgr.h"

CMainGame::CMainGame()
	: m_ullTime(GetTickCount64()), m_iFPS(0), m_hDC(nullptr)
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

	// m_pPlayer = CAbstractFactory<CPlayer>::Create();
	// dynamic_cast<CPlayer*>(m_pPlayer)->Set_Bullet(&m_BulletList);

	//게임 시작 화면 출력

	//while (true)
	//{
	//	TCHAR szBuf[32] = L"";
	//	swprintf_s(szBuf, L"시작하려면 아무키나 누르세요.");
	//	TextOut(m_hDC, 300, 450, szBuf, lstrlen(szBuf));

	//	if (GetAsyncKeyState(VK_SPACE))
	//	{
	//		break;
	//	}
	//}


	m_ObjList[OBJ_PLAYER].push_back(CAbstractFactory<CPlayer>::Create());
	dynamic_cast<CPlayer*>(m_ObjList[OBJ_PLAYER].front())->Set_Bullet(&m_ObjList[OBJ_BULLET]);

	for (int i = 0; i < 3; ++i)
	{
		float fRandX = rand() % GAME_WIN_RIGHT;
		m_ObjList[OBJ_MONSTER].push_back(CAbstractFactory<CMonster>::Create(fRandX, 200.f, DIR_END));
		
	}

}

void CMainGame::Update()
{
#pragma region  복습

	// m_pPlayer->Update();
	// 
	// for (auto iter = m_BulletList.begin();
	// 	iter != m_BulletList.end(); )
	// {
	// 	int iResult = (*iter)->Update();
	// 
	// 	if (OBJ_DEAD == iResult)
	// 	{
	// 		Safe_Delete<CObj*>(*iter);
	// 		iter = m_BulletList.erase(iter);
	// 	}
	// 	else
	// 		++iter;
	// }

#pragma endregion

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); )
		{
			int iResult = (*iter)->Update();

			if (OBJ_DEAD == iResult)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

}

void CMainGame::Late_Update()
{
#pragma region  복습
	/*m_pPlayer->Late_Update();

	for (auto& pBullet : m_BulletList)
		pBullet->Late_Update();*/
#pragma endregion

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			pObj->Late_Update();
	}

	CCollisionMgr::Collision_Circle(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_BULLET]);
}

void CMainGame::Render()
{
#pragma region  FPS 출력
	++m_iFPS;

	if (m_ullTime + 1000 < GetTickCount64())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_ullTime = GetTickCount64();
	}
#pragma endregion
	
	Rectangle(m_hDC, 0, 0, WINCX, WINCY);
	Rectangle(m_hDC, int(GAME_WIN_LEFT), int(GAME_WIN_TOP), int(GAME_WIN_RIGHT), int(GAME_WIN_BOTTOM));
#pragma region  복습
	/*m_pPlayer->Render(m_hDC);

	for (auto& pBullet : m_BulletList)
		pBullet->Render(m_hDC);*/
#pragma endregion

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			pObj->Render(m_hDC);
	}
}

void CMainGame::Release()
{
#pragma region 복습
	//Safe_Delete<CObj*>(m_pPlayer);

//for_each(m_BulletList.begin(), m_BulletList.end(), Safe_Delete<CObj*>);
//m_BulletList.clear();

#pragma endregion


	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}

	ReleaseDC(g_hWnd, m_hDC);
}
