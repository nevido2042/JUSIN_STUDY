#include "pch.h"
#include "CAstarMgr.h"
#include "ObjMgr.h"
#include "CDevice.h"

IMPLEMENT_SINGLETON(CAstarMgr)

CAstarMgr::CAstarMgr()
{
}

CAstarMgr::~CAstarMgr()
{
	Release();
}

void CAstarMgr::Start_Astar(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal)
{
	Release();

	CObj* pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	m_iStartIdx = Get_TileIdx(vStart);

	int iGoalIdx = Get_TileIdx(vGoal);

	if (0 > m_iStartIdx							||
		0 > iGoalIdx							||
		(size_t)m_iStartIdx >= vecTile.size()	||
		(size_t)iGoalIdx >= vecTile.size())
	{
		return;
	}

	if (m_iStartIdx == iGoalIdx)
		return;

	if (0 != vecTile[iGoalIdx]->byOption)
		return;

	if (true == Make_Route(m_iStartIdx, iGoalIdx))
	{
		Make_BestList(m_iStartIdx, iGoalIdx);
		Find_ShortestPath_Bresenham();
	}

}

void CAstarMgr::Render()
{
	Render_BestList();
	Render_BestestList();
}


bool CAstarMgr::Make_Route(int iStartIdx, int iGoalIdx)
{
	CObj* pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();
	vector<list<TILE*>>& vecAdj = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecAdj();

	if (!m_OpenList.empty())
		m_OpenList.pop_front();

	m_CloseList.push_back(iStartIdx);

	for (auto& pTile : vecAdj[iStartIdx])
	{
		if (iGoalIdx == pTile->iIndex)
		{
			pTile->iParentIndex = iStartIdx;
			return true;
		}

		if (false == Check_Close(pTile->iIndex) &&
			false == Check_Open(pTile->iIndex))
		{
			pTile->iParentIndex = iStartIdx;

			m_OpenList.push_back(pTile->iIndex);
		}
	}

	if (m_OpenList.empty())
		return false;

	int iStart = m_iStartIdx;

	m_OpenList.sort([&vecTile, &iGoalIdx, &iStart](int Dst, int Src)->bool
	{
		D3DXVECTOR3	vPCost1 = vecTile[iStart]->vPos - vecTile[Dst]->vPos;
		D3DXVECTOR3	vPCost2 = vecTile[iStart]->vPos - vecTile[Src]->vPos;

		D3DXVECTOR3	vGCost1 = vecTile[iGoalIdx]->vPos - vecTile[Dst]->vPos;
		D3DXVECTOR3	vGCost2 = vecTile[iGoalIdx]->vPos - vecTile[Src]->vPos;

		float	fCost1 = D3DXVec3Length(&vPCost1) + D3DXVec3Length(&vGCost1);
		float	fCost2 = D3DXVec3Length(&vPCost2) + D3DXVec3Length(&vGCost2);

		return fCost1 < fCost2;
	});

	return Make_Route(m_OpenList.front(), iGoalIdx);
}

void CAstarMgr::Make_BestList(int iStartIdx, int iGoalIdx)
{
	CObj* pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	m_BestList.push_front(vecTile[iGoalIdx]);

	int iRouteIdx = vecTile[iGoalIdx]->iParentIndex;

	while (true)
	{
		if (iRouteIdx == iStartIdx)
			break;

		m_BestList.push_front(vecTile[iRouteIdx]);
		iRouteIdx = vecTile[iRouteIdx]->iParentIndex;
	}
}

void CAstarMgr::Find_ShortestPath_Bresenham()
{
	/*
	현재노드, 체크노드
	노드 카운트

	while(true)
	{
		if(현재노드 == nullptr && 체크노드 == nullptr)
		{
			현재노드 = 처음 노드
			체크노드 = 두 번째 노드
			continue;
		}

		현재노드와 체크노드 사이를 브레즌햄 알고리즘으로 장애물 체크

		if(장애물이 없으면)
		{
			++노드 카운트
			 체크 노드에 체크노드의 다음 노드 저장
		}
		else if(장애물이 있으면)
		{
			 현재노드의 앞의 노드 부터 (노드카운트-1) 만큼 삭제
			 현재 노드에 채크 노드 저장, 체크 노드에 체크노드의 다음 노드 저장
		}

		if(체크노드에 end가 들어 가있으면) break;
	}
	*/

	// 최적화할 경로 리스트 (A* 결과)
	vector<TILE*> path(m_BestList.begin(), m_BestList.end());

	if (path.empty() || path.size() < 3)
		return;

	TILE* currTile(nullptr);
	TILE* checkTile(nullptr);
	int iCount(0);
	int iCurrIndex(0);
	int iCheckIndex(0);

	// 반복문을 돌며 경로를 최적화
	while (true)
	{
		// 초기화: 처음 두 타일 설정
		if (currTile == nullptr && checkTile == nullptr)
		{
			currTile = path[0];
			checkTile = path[1];
			iCurrIndex = 0;
			iCheckIndex = 1;
			if (!currTile || !checkTile)
			{
				break;
			}

			continue;
		}

		// 장애물 체크 (브레젠험 알고리즘 사용)
		bool hasObstacle = Check_Obstacle_Bresenham(currTile->iIndex, checkTile->iIndex);

		if (!hasObstacle)
		{
			//++카운트
			//체크 노드에 체크노드의 다음 노드 저장
			++iCount;
			++iCheckIndex;
			checkTile = path[iCheckIndex];
		}
		else
		{
			//현재노드의 앞의 노드 부터(노드카운트 - 1) 만큼 삭제
			//현재 노드에 채크 노드 저장, 체크 노드에 체크노드의 다음 노드 저장
			auto iter = path.begin() + iCurrIndex + 1;
			for (int i = 0; i < iCount - 1; ++i)
			{
				iter = path.erase(iter);
			}

			iCount = 0;
			iCurrIndex = iCheckIndex;
			currTile = checkTile;
			++iCheckIndex;
			if (path.size() > iCheckIndex)
			{
				checkTile = path[iCheckIndex];
			}
			else
			{
				break;
			}

		}

		// 종료 조건: 최종 타일에 도달
		if (checkTile == path.back())
			break;
	}

	// 최적화된 경로 저장
	m_BestestList.assign(path.begin(), path.end());
	//m_BestList.assign(path.begin(), path.end());
}

bool CAstarMgr::Check_Obstacle_Bresenham(int _iStartIndex, int _iEndIndex)
{
	

	int x0 = static_cast<int>(_iStartIndex % TILEX);
	int y0 = static_cast<int>(_iStartIndex / TILEX);
	int x1 = static_cast<int>(_iEndIndex % TILEX);
	int y1 = static_cast<int>(_iEndIndex / TILEX);

	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;

	while (true)
	{
		if (x0 < 0 || y0 < 0 || x0 >TILEX - 1 || y0 > TILEY - 1)
			break;

		// 장애물 확인
		if (IsObstacle(x0, y0)) // 예: 특정 좌표에 장애물이 있으면 true 반환
			return true;

		if (x0 == x1 && y0 == y1)
			break;

		e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			y0 += sy;
		}
	}

	return false; // 장애물 없음
}

bool CAstarMgr::IsObstacle(int _iX, int _iY)
{
	CObj* pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();
	
	if (vecTile[_iY * TILEX + _iX]->byOption == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}



int CAstarMgr::Get_TileIdx(const D3DXVECTOR3& vPos)
{
	CObj* pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	if (vecTile.empty())
		return -1;

	for (size_t index = 0; index < vecTile.size(); ++index)
	{
		if (Picking(vPos, index))
		{
			return index;
		}
	}

	return -1;
}

bool CAstarMgr::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
{
	CObj* pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	// 12 -> 3 -> 6 -> 9
	D3DXVECTOR3	vPoint[4] =
	{
		{ vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ vecTile[iIndex]->vPos.x + (TILECX / 2.f), vecTile[iIndex]->vPos.y, 0.f },
		{ vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ vecTile[iIndex]->vPos.x - (TILECX / 2.f), vecTile[iIndex]->vPos.y, 0.f },

	};

	D3DXVECTOR3	vDir[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	D3DXVECTOR3		vNormal[4] =
	{
		{ -vDir[0].y , vDir[0].x, 0.f },
		{ -vDir[1].y , vDir[1].x, 0.f },
		{ -vDir[2].y , vDir[2].x, 0.f },
		{ -vDir[3].y , vDir[3].x, 0.f },
	};

	D3DXVECTOR3		vMouseDir[4] =
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}
	return true; 
}

bool CAstarMgr::Check_Close(int iIndex)
{
	for (int& iCloseIdx : m_CloseList)
	{
		if (iIndex == iCloseIdx)
			return true;
	}

	return false;
}

bool CAstarMgr::Check_Open(int iIndex)
{
	for (int& iOpenIdx : m_OpenList)
	{
		if (iIndex == iOpenIdx)
			return true;
	}

	return false;
}

void CAstarMgr::Render_BestList()
{
	if (m_BestList.empty())
	{
		return;
	}

	// 디바이스 가져오기
	LPDIRECT3DDEVICE9 pDevice = CDevice::Get_Instance()->Get_Device();
	if (!pDevice)
		return;

	// ID3DXLine 생성
	CDevice::Get_Instance()->CreateLine();
	ID3DXLine* pLine = CDevice::Get_Instance()->Get_Line();
	if (!pLine)
		return;

	pLine->SetWidth(2.0f);               // 선의 굵기

	//이전 타일
	TILE* pPrevTile(nullptr);
	//인덱스 01, 12, 23, 34 ... 타일을 이어라
	for (TILE* pTile : m_BestList)
	{
		//처음 시작 타일일 경우 이전타일이 없음
		if (pPrevTile == nullptr)
		{
			pPrevTile = pTile;
			continue;
		}

		// 화면 좌표로 선 두 점 정의
		D3DXVECTOR2 LinePoints[] = {
			D3DXVECTOR2(pPrevTile->vPos.x + CObj::Get_Scroll().x, pPrevTile->vPos.y + CObj::Get_Scroll().y),  // 시작점
			D3DXVECTOR2(pTile->vPos.x + CObj::Get_Scroll().x ,pTile->vPos.y + CObj::Get_Scroll().y)			// 끝점
		};

		// 선 그리기 준비
		pLine->Begin();

		// 선 그리기 (흰색)
		pLine->Draw(LinePoints, 2, D3DCOLOR_XRGB(255, 0, 0));

		// 선 그리기 종료
		pLine->End();

		pPrevTile = pTile;
	}

}

void CAstarMgr::Render_BestestList()
{
	if (m_BestestList.empty())
	{
		return;
	}

	// 디바이스 가져오기
	LPDIRECT3DDEVICE9 pDevice = CDevice::Get_Instance()->Get_Device();
	if (!pDevice)
		return;

	// ID3DXLine 생성
	CDevice::Get_Instance()->CreateLine();
	ID3DXLine* pLine = CDevice::Get_Instance()->Get_Line();
	if (!pLine)
		return;

	pLine->SetWidth(2.0f);               // 선의 굵기

	//이전 타일
	TILE* pPrevTile(nullptr);
	//인덱스 01, 12, 23, 34 ... 타일을 이어라
	for (TILE* pTile : m_BestestList)
	{
		//처음 시작 타일일 경우 이전타일이 없음
		if (pPrevTile == nullptr)
		{
			pPrevTile = pTile;
			continue;
		}

		// 화면 좌표로 선 두 점 정의
		D3DXVECTOR2 LinePoints[] = {
			D3DXVECTOR2(pPrevTile->vPos.x + CObj::Get_Scroll().x, pPrevTile->vPos.y + CObj::Get_Scroll().y),  // 시작점
			D3DXVECTOR2(pTile->vPos.x + CObj::Get_Scroll().x ,pTile->vPos.y + CObj::Get_Scroll().y)			// 끝점
		};

		// 선 그리기 준비
		pLine->Begin();

		// 선 그리기 (흰색)
		pLine->Draw(LinePoints, 2, D3DCOLOR_XRGB(0, 255, 0));

		// 선 그리기 종료
		pLine->End();

		pPrevTile = pTile;
	}

}

void CAstarMgr::Release()
{
	m_OpenList.clear();
	m_CloseList.clear();

	m_BestList.clear();
	m_BestestList.clear();
}
