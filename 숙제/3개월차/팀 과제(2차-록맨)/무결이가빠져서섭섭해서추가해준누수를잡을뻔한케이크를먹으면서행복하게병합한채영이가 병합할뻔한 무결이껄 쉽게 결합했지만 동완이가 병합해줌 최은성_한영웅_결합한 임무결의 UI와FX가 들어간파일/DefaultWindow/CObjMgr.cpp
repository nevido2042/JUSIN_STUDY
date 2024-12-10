#include "pch.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"
#include "CMonster.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

CObj* CObjMgr::Get_Target(OBJID eID, CObj* pDst)
{
	if (m_ObjList[eID].empty())
		return nullptr;

	CObj*	pTarget = nullptr;
	float	fDistance(0.f);

	for (auto& Src : m_ObjList[eID])
	{
		if (Src->Get_Dead())
			continue;

		float		fWidth = abs(pDst->Get_Info().fX - Src->Get_Info().fX);
		float		fHeight = abs(pDst->Get_Info().fY - Src->Get_Info().fY);

		float		fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		if ((!pTarget) || (fDistance > fDiagonal))
		{
			pTarget = Src;
			fDistance = fDiagonal;
		}
	}		

	return pTarget;
}

void CObjMgr::Add_Object(OBJID eID, CObj* pObj)
{
	if (OBJ_END <= eID || nullptr == pObj)
		return;

	m_ObjList[eID].push_back(pObj);
}

int CObjMgr::Update()
{
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

	return 0;
}

void CObjMgr::Late_Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			pObj->Late_Update();
	}

	if (!m_ObjList[OBJ_PLAYER].empty())
		CCollisionMgr::Collision_Player_Monster(m_ObjList[OBJ_PLAYER].front(), m_ObjList[OBJ_MONSTER]);
	
	// 플레이어 총알 충돌
	CCollisionMgr::Collision_Circle(m_ObjList[OBJ_BULLET], m_ObjList[OBJ_MONSTER]);
	CCollisionMgr::Collision_Circle(m_ObjList[OBJ_BULLET], m_ObjList[OBJ_BOSS]);

	// 적 총알 충돌
	CCollisionMgr::Collision_Circle(m_ObjList[OBJ_BOSSBULLET], m_ObjList[OBJ_PLAYER]);


	// 여기까지
	// 이 아래는 은성 테스트 중인 코드
	CCollisionMgr::Collision_Floor(m_ObjList[OBJ_PLAYER].front(), m_ObjList[OBJ_BLOCK]);
	CCollisionMgr::Collision_Celling(m_ObjList[OBJ_PLAYER].front(), m_ObjList[OBJ_BLOCK]);
	CCollisionMgr::Collision_Box(m_ObjList[OBJ_PLAYER].front(), m_ObjList[OBJ_BLOCK]);

	for (auto Boss : m_ObjList[OBJ_BOSS])
	{
		CCollisionMgr::Collision_Floor(Boss, m_ObjList[OBJ_BLOCK]);
		CCollisionMgr::Collision_Celling(Boss, m_ObjList[OBJ_BLOCK]);
		CCollisionMgr::Collision_Box(Boss, m_ObjList[OBJ_BLOCK]);
	}

	for (auto Monster : m_ObjList[OBJ_MONSTER])

	{
		CCollisionMgr::Collision_Floor(Monster, m_ObjList[OBJ_BLOCK]);
		CCollisionMgr::Collision_Celling(Monster, m_ObjList[OBJ_BLOCK]);
		CCollisionMgr::Collision_Box(Monster, m_ObjList[OBJ_BLOCK]);
	}
}

void CObjMgr::Render(HDC hDC)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			pObj->Render(hDC);
	}
}

void CObjMgr::Release()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
}
