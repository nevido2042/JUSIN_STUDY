#include "pch.h"
#include "StageScene.h"
#include "Player.h"
#include "Monster.h"
#include "Satellite.h"
#include "Portion.h"
#include "AddBullet.h"
#include "AddSatellite.h"
#include "CollisionMgr.h"

StageScene::StageScene() : m_dwTime(0), bFinish(false)
{
}

StageScene::~StageScene()
{
	Release();
}

void StageScene::Initialize(Obj* _pPlayer)
{
	m_ObjList[OBJ_PLAYER].push_back(_pPlayer);
	static_cast<Player*>(m_ObjList[OBJ_PLAYER].front())->Set_Bullet(&m_ObjList[OBJ_PLAYER_BULLET]);
	
	m_dwTime = GetTickCount64();
}

int StageScene::Update()
{
	if (bFinish) {
		return OBJ_CLEAR;
	}
	int tmp = rand() % 4; // 난수로 랜덤하게 아이템 생성
	SpawnMonster();
	m_ObjList[OBJ_MONSTER].back()->Set_Target(m_ObjList[OBJ_PLAYER].front());

	for (size_t i = 0; i < OBJ_END; ++i) {
		for (auto iter = m_ObjList[i].begin(); iter != m_ObjList[i].end();) {
			int result = (*iter)->Update();

			if (OBJ_DEAD == result) {
				if (dynamic_cast<Player*>(*iter)) {
					return OBJ_DEAD;
				}
				else {
					if (i == OBJ_MONSTER)
					{
						if (tmp == 0)
						{
							Obj* pPortion = new Portion;
							pPortion->Initialize();
							pPortion->Set_Pos((*iter)->Get_Info().fX, (*iter)->Get_Info().fY);
							pPortion->Set_Target(m_ObjList[OBJ_PLAYER].front());
							m_ObjList[OBJ_ITEM].push_back(pPortion);
						}
						else if(tmp == 1)
						{
							Obj* pItem = new AddBullet;
							pItem->Initialize();
							pItem->Set_Pos((*iter)->Get_Info().fX, (*iter)->Get_Info().fY);
							pItem->Set_Target(m_ObjList[OBJ_PLAYER].front());
							m_ObjList[OBJ_ITEM].push_back(pItem);
						}
						else if(tmp == 2)
						{
							Obj* pItem = new AddSatellite;
							pItem->Initialize();
							pItem->Set_Pos((*iter)->Get_Info().fX, (*iter)->Get_Info().fY);
							pItem->Set_Target(m_ObjList[OBJ_PLAYER].front());
							m_ObjList[OBJ_ITEM].push_back(pItem);
						}
						
					}
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

void StageScene::Late_Update()
{
	CollisionMgr::Damage_Circle(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_PLAYER_BULLET]);
	CollisionMgr::Damage_Circle(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_PLAYER]);
	CollisionMgr::Damage_Circle(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_SATELLITE]);

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			pObj->Late_Update();
	}



	
	if (!m_ObjList[OBJ_ITEM].empty())
	{
		for (auto& i : m_ObjList[OBJ_ITEM])
		{
			auto iter = m_ObjList[OBJ_PLAYER].begin();
			if (CollisionMgr::Collision_Circle(*iter, i))
			{
				//useItem 만들어야 겠다...
				//(*iter)->Set_Hp((*iter)->Get_Hp() + 10);
				
				if (nullptr == dynamic_cast<Item*>(i)->UseItem())
				{
					i->Set_Dead();
				}
				else
				{
					m_ObjList[OBJ_SATELLITE].push_back(dynamic_cast<Item*>(i)->UseItem());
					m_ObjList[OBJ_SATELLITE].back()->Set_Target(m_ObjList[OBJ_PLAYER].front());
					Obj* pTarget = m_ObjList[OBJ_SATELLITE].back()->Get_Target();
					m_ObjList[OBJ_SATELLITE].back()->Set_Pos(pTarget->Get_Info().fX, pTarget->Get_Info().fY);
					
					i->Set_Dead();
				}
				
				
			}
				
		}
	}
		
	
	


	

	if (m_ObjList[OBJ_MONSTER].size() == 0) {
		bFinish = true;
	}
}

void StageScene::Render(HDC _hDC)
{

	Rectangle(_hDC, int(GAME_WIN_LEFT), int(GAME_WIN_TOP), int(GAME_WIN_RIGHT), int(GAME_WIN_BOTTOM));
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			pObj->Render(_hDC);
	}

	TCHAR szBullet[32];
	wsprintf(szBullet, L"Bullet: %d", (int)m_ObjList[OBJ_PLAYER_BULLET].size());
	TextOut(_hDC, 50, 50, szBullet, lstrlen(szBullet));

	TCHAR szMonster[32];
	wsprintf(szMonster, L"Monster: %d", (int)m_ObjList[OBJ_MONSTER].size());
	TextOut(_hDC, 150, 50, szMonster, lstrlen(szMonster));

	TCHAR szHp[32];
	wsprintf(szHp, L"Hp: %d", (*m_ObjList[OBJ_PLAYER].front()).Get_Hp());
	TextOut(_hDC, 250, 50, szHp, lstrlen(szHp));
}

void StageScene::Release() 
{
	for (size_t i = 1; i < OBJ_END; ++i) {
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<Obj*>);
		m_ObjList->clear();
	}
}

void StageScene::SpawnMonster()
{
	if (m_dwTime + 1000 < GetTickCount64()) {
		
		m_ObjList[OBJ_MONSTER].push_back(new Monster());
		m_ObjList[OBJ_MONSTER].back()->Initialize();
		m_dwTime = GetTickCount64();
	}
}
