#include "pch.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Camera.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

CObj* CObjMgr::Find_Obj(const TCHAR* _pImgKey, OBJID _eOBJID)
{
	//같은 이미지 키를 여러게가 공유하고 있으면 가장
	//처음 찾은 놈으로 나온다.
	CObj* pResult(nullptr);

	//이미지 키가 같은 녀석이 나오면 리턴
	for (CObj* pObj : Get_List()[_eOBJID])
	{
		if (!lstrcmp(_pImgKey, pObj->Get_ImgKey()))
		{
			pResult = pObj;
			break;
		}
	}

	return pResult;
}

void CObjMgr::Add_Object(OBJID eID, CObj* pObj)
{
	if (eID < 0 || OBJ_END <= eID || nullptr == pObj)
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

			if (OBJ_DESTROYED == iResult)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return OBJ_NOEVENT;
}

void CObjMgr::Late_Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
		{
			/*if (OBJ_SETSCENE == pObj->Late_Update())
				break;*/

			pObj->Late_Update();

			if (m_ObjList[i].empty())
				break;

			RENDERID	eID = pObj->Get_GroupID();
			m_RenderList[eID].push_back(pObj);
		}
	}

	//CCollisionMgr::Collision_Rect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_BULLET]);
	//CCollisionMgr::Collision_Circle(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_BULLET]);
	//CCollisionMgr::Collision_Circle(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_MOUSE]);
	//CCollisionMgr::Collision_RectEx(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_PLAYER]);

	CCollisionMgr::Collision_Circle(OBJ_PROJECTILE, OBJ_RIM);
	CCollisionMgr::Collision_Circle(OBJ_PROJECTILE, OBJ_ENEMY);
	CCollisionMgr::Collision_Circle(OBJ_PROJECTILE, OBJ_WALL);

}

void CObjMgr::Render(HDC hDC)
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		if (i != RENDER_UI && i != RENDER_INUI)//UI는 Y축 정렬 없애기
		{
			m_RenderList[i].sort([](CObj* pDst, CObj* pSrc)->bool
				{
					return pDst->Get_Info().fY < pSrc->Get_Info().fY;
				});
		}

		for (auto& pObj : m_RenderList[i])
		{
			if (CCamera::Get_Instance()->IsInCameraView(pObj->Get_Info().fX, pObj->Get_Info().fY, pObj->Get_Info().fCX, pObj->Get_Info().fCY) ||
				pObj->Get_GroupID() == RENDER_UI || pObj->Get_GroupID() == RENDER_INUI)
			{
				pObj->Render(hDC);
			}
		}


		m_RenderList[i].clear();
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

void CObjMgr::Delete_ID(OBJID eID)
{
	/*for (auto& pObj : m_ObjList[eID])
		Safe_Delete(pObj);

	m_ObjList[eID].clear();*/

	for (auto& pObj : m_ObjList[eID])
		pObj->Set_Destroyed();
}

void CObjMgr::Delete_All()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			pObj->Set_Destroyed();
	}
}
