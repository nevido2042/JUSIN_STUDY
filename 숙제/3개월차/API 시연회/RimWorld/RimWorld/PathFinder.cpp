#include "pch.h"
#include "PathFinder.h"
#include "Node.h"
#include "TileMgr.h"

CPathFinder* CPathFinder::m_pInstance = nullptr;

CPathFinder::CPathFinder()
{
}

CPathFinder::~CPathFinder()
{
}

list<CNode*> CPathFinder::Find_Path(POS _tStart, POS _tEnd)
{
	CNode* StartNode = new CNode(_tStart);
	m_OpenList.push_back(StartNode);

	while (!m_OpenList.empty())
	{
		//하나 뽑는다.
		CNode* pPopNode = m_OpenList.front();
		m_OpenList.pop_front();

		//도착 지점인지 확인한다.
		if (CNode::Distance(_tEnd, pPopNode->Get_Pos()) < TILECX)
		{
			

			//클로즈리스트를 반환 해야할까? 리스트를 넘겨줘야할까?

			return m_CloseList;
		}

		//클로즈 리스트에 넣는다.
		m_CloseList.push_back(pPopNode);

		//8방향 탐색한다.
		int iDX[8]{ 0, 1, 1, 1,	0, -1, -1, -1 };
		int iDY[8]{ -1,	-1, 0, 1, 1, 1,	0, -1 };
		//  12   1  3  4  6   7   9  11
		
		for (int i = 0; i < 8; ++i)
		{
			//CTileMgr::Get_Instance()->Get_TileArray().at(1);
			float fX = float(pPopNode->Get_Pos().fX + iDX[i] * TILECX);
			float fY = float(pPopNode->Get_Pos().fY + iDY[i] * TILECY);

			//맵 밖인가?
			if (fX < 0 || fY < 0 || TILECX * TILEX < fX || TILECY * TILEY < fY)
			{
				continue;
			}

			POS NewPos(fX, fY);

			//fx fy로 인덱스 계산
			int iIdx = int(fX / TILECX) + int((fY / TILECY) * TILEX);

			//벽인가?
			CTile* pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->
				Get_TileArray().at(iIdx));

			if (!pTile || pTile->Get_Option() == OPT_BLOCKED)
			{
				continue;
			}

			//클로즈 리스트 또는 오픈리스트에 존재 하는가?
			bool bIsNewPos = true;
			for (list<CNode*>::iterator iter = m_OpenList.begin(); iter != m_OpenList.end(); ++iter)
			{
				if (NewPos == (*iter)->Get_Pos())
				{
					bIsNewPos = false;
					break;
				}
			}
			for (list<CNode*>::iterator iter = m_CloseList.begin(); iter != m_CloseList.end(); ++iter)
			{
				if (NewPos == (*iter)->Get_Pos())
				{
					bIsNewPos = false;
					break;
				}
			}
			if (bIsNewPos)
			{
				CNode* NewNode = new CNode(NewPos, pPopNode, _tEnd);
				m_OpenList.push_back(NewNode);
			}
		}

		//오픈 리스트 정렬
		m_OpenList.sort(LessF);

		//m_pMap->Update();
	}

	return m_CloseList;//실패
}

bool CPathFinder::LessF(const CNode* _First, const CNode* _Second)
{
	return _First->Get_F() < _Second->Get_F();
}
