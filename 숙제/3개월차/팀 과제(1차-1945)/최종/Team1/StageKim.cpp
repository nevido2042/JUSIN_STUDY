#include "pch.h"
#include "StageKim.h"
#include "KBossMonster.h"
#include "Player.h"
#include "CollisionMgr.h"



StageKim::StageKim()
{
}

StageKim::~StageKim()
{
}

void StageKim::Initialize()
{
}

void StageKim::SpawnMonster()
{
	m_ObjList[OBJ_MONSTER].push_back(new KBossMonster());
	m_ObjList[OBJ_MONSTER].back()->Initialize();
	m_ObjList[OBJ_MONSTER].back()->Set_Target(m_ObjList[OBJ_PLAYER].front());
	static_cast<KBossMonster*>(m_ObjList[OBJ_MONSTER].back())->Set_Bullet(&m_ObjList[OBJ_BULLET_MONSTER]);
}

int StageKim::Update()
{
	if (m_bFinish) {
		return OBJ_CLEAR;
	}
	if (m_bStart)
	{
		m_ulStartTime = GetTickCount64();
		m_ulStartTime += 60000;
		SpawnMonster();

		// 2개
		/*auto iter = m_ObjList[OBJ_MONSTER].begin();
		(*iter)->Set_Pos((*iter)->Get_Info().fX - 100, (*iter)->Get_Info().fY);
		SpawnMonster();
		++iter;
		(*iter)->Set_Pos((*iter)->Get_Info().fX + 100, (*iter)->Get_Info().fY);*/


		m_bStart = false;
	}

	

	for (size_t i = 0; i < OBJ_END; ++i) {
		for (auto iter = m_ObjList[i].begin(); iter != m_ObjList[i].end();) {

			int result = (*iter)->Update();

			if (OBJ_DEAD == result) {
				if (dynamic_cast<Player*>(*iter)) {
					return OBJ_DEAD;
				}
				else if (dynamic_cast<Monster*>(*iter)) {
					SpawnItem((*iter)->Get_Info().fX, (*iter)->Get_Info().fY);
					Safe_Delete<Obj*>(*iter);
					iter = m_ObjList[i].erase(iter);
				}
				else {
					Safe_Delete<Obj*>(*iter);
					iter = m_ObjList[i].erase(iter);
				}
			}
			else {
				++iter;
			}
		}
	}

	
	return OBJ_NOEVENT;
}

void StageKim::Late_Update()
{
	// 아이템 충돌 제외
	CollisionMgr::Collision_Rect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_PLAYER]); //몬스터 & 플레이어
	CollisionMgr::Collision_Rect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_BULLET_PLAYER]); //몬스터 & 총알
	CollisionMgr::Collision_Circle(m_ObjList[OBJ_SHIELD], m_ObjList[OBJ_MONSTER]); //몬스터 & 쉴드

	CollisionMgr::Collision_Circle(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_BULLET_MONSTER]); // 플레이어 & 총알
	CollisionMgr::Collision_Circle(m_ObjList[OBJ_SHIELD], m_ObjList[OBJ_BULLET_MONSTER]); // 실드 & 총알


	if ((m_ulStartTime - GetTickCount64()) / 1000 <= 0) {
		m_bFinish = true;
	}

	if (m_ObjList[OBJ_MONSTER].size() == 0) {
		m_bFinish = true;
	}

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			pObj->Late_Update();
	}

	
}

void StageKim::Render(HDC _hDC)
{
	Rectangle(_hDC, int(GAME_WIN_LEFT), int(GAME_WIN_TOP), int(GAME_WIN_RIGHT), int(GAME_WIN_BOTTOM));

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			pObj->Render(_hDC);
	}

	TCHAR szBullet[32];
	wsprintf(szBullet, L"Pbullet: %d", (int)m_ObjList[OBJ_BULLET_PLAYER].size());
	TextOut(_hDC, 50, 50, szBullet, lstrlen(szBullet));

	TCHAR szMBullet[32];
	wsprintf(szMBullet, L"Mbullet: %d", (int)m_ObjList[OBJ_BULLET_MONSTER].size());
	TextOut(_hDC, 150, 50, szMBullet, lstrlen(szMBullet));

	TCHAR szMonster[32];
	wsprintf(szMonster, L"Monster: %d", (int)m_ObjList[OBJ_MONSTER].size());
	TextOut(_hDC, 250, 50, szMonster, lstrlen(szMonster));

	TCHAR szPlayerHp[32];
	wsprintf(szPlayerHp, L"PlayerHp: %d", m_ObjList[OBJ_PLAYER].front()->Get_Hp());
	TextOut(_hDC, 350, 50, szPlayerHp, lstrlen(szPlayerHp));

	if (!m_ObjList[OBJ_MONSTER].empty())
	{
		TCHAR szBossHp[32];
		wsprintf(szBossHp, L"BossHp: %d", m_ObjList[OBJ_MONSTER].front()->Get_Hp());
		TextOut(_hDC, 600, 50, szBossHp, lstrlen(szBossHp));
	}
	


	TCHAR szTimer[32];
	wsprintf(szTimer, L"Time: %d", int((m_ulStartTime - GetTickCount64()) / 1000));
	TextOut(_hDC, 450, 50, szTimer, lstrlen(szTimer));
}
