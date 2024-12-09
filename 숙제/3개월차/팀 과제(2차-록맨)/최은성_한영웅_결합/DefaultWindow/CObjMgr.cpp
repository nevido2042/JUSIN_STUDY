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
	// 원충돌 기반 삭제
	// CCollisionMgr::Collision_Circle(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_BULLET]);
	//CCollisionMgr::Collision_Circle(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_MOUSE]);
	
	//CCollisionMgr::Collision_Rect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_BULLET]);


	//블럭과 플레이어 충돌(플레이어가 밀려나감)
	//CCollisionMgr::Collision_RectEx(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_BLOCK]);
	//CCollisionMgr::Collision_RectEx(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_PLAYER]);



	//이 아래가 원래 있던 코드
	//CCollisionMgr::Collision_RectEx(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_WALL]);
	//// 땅 충돌
	//if (m_ObjList[OBJ_PLAYER].empty() == false)
	//{
	//	CCollisionMgr::Collision_Jump(m_ObjList[OBJ_PLAYER].front(), m_ObjList[OBJ_BLOCK]);
	//	CCollisionMgr::Collision_Box(m_ObjList[OBJ_PLAYER].front(), m_ObjList[OBJ_BLOCK]);
	//}

	//if( m_ObjList[OBJ_BOSS].empty()==false)
	//	CCollisionMgr::Collision_Boss_Box(m_ObjList[OBJ_BOSS].front(), m_ObjList[OBJ_BLOCK]);

	// 여기까지
	// 이 아래는 은성 테스트 중인 코드


	CCollisionMgr::Collision_Floor(m_ObjList[OBJ_PLAYER].front(), m_ObjList[OBJ_BLOCK]);
	CCollisionMgr::Collision_Box(m_ObjList[OBJ_PLAYER].front(), m_ObjList[OBJ_BLOCK]);
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
