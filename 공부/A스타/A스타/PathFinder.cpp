#include "pch.h"
#include "PathFinder.h"
#include <algorithm>

CPathFinder::CPathFinder():m_pMap(nullptr)
{
}

CPathFinder::~CPathFinder()
{
	Release();
}

void CPathFinder::Initialize(CMap& _pMap)
{
	m_pMap = &_pMap;
	m_StartPos = _pMap.Get_StartPos();
	m_EndPos = _pMap.Get_EndPos();
}

void CPathFinder::Release()
{
	for_each(m_OpenList.begin(), m_OpenList.end(), Safe_Delete<CNode*>);
	for_each(m_CloseList.begin(), m_CloseList.end(), Safe_Delete<CNode*>);

	/*for (int i = 0; i < m_OpenList.size(); ++i)
	{
		Safe_Delete<CNode*>(m_OpenList.top());
	}*/

	/*for (int i = 0; i < m_CloseList.size(); ++i)
	{
		Safe_Delete<CNode*>(m_CloseList.top());
	}*/
}

bool CPathFinder::LessF(const CNode* _First, const CNode* _Second)
{
	return _First->Get_F() < _Second->Get_F();
}

//bool CPathFinder::LessF(const CNode& _First, const CNode& _Second)
//{
//	return _First.Get_F() < _Second.Get_F();
//}

void CPathFinder::Start()
{
	CNode* StartNode = new CNode(m_StartPos);
	m_OpenList.push_back(StartNode);
	
	while (!m_OpenList.empty())
	{
		//하나 뽑는다.
		CNode* PopNode = m_OpenList.front();
		m_OpenList.pop_front();

		//도착 지점인지 확인한다.
		if (m_EndPos == PopNode->Get_Pos())
		{
			m_pMap->Change_Tile(m_EndPos, END);
			// 부모를 이어서 길을 만든다.
			CNode* pParent = PopNode;
			while (pParent = pParent->Get_Parent())
			{
				//시작지점은 색칠하지 않도록
				if (pParent->Get_Pos() == m_StartPos)
					break;

				m_pMap->Change_Tile(pParent->Get_Pos(), PATH);
				m_pMap->Update();
				Sleep(10);
			}

			return;
		}

		//클로즈 리스트에 넣는다.
		m_CloseList.push_back(PopNode);

		//8방향 탐색한다.
		int iDX[8]{ 0, 1, 1, 1,	0, -1, -1, -1 };
		int iDY[8]{	-1,	-1, 0, 1, 1, 1,	0, -1 };
				//  12   1  3  4  6   7   9  11

		for (int i = 0; i < 8; ++i)
		{
			int iX = PopNode->Get_Pos().iX + iDX[i];
			int iY = PopNode->Get_Pos().iY + iDY[i];

			//맵 밖인가?
			if (iX < 0 || iY < 0 || m_pMap->Get_Width() - 1 < iX || m_pMap->Get_Height() - 1 < iY)
			{
				continue;
			}

			POS NewPos(iX, iY);

			//벽인가?
			if (m_pMap->Get_Tile_Type(NewPos) == WALL)
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
				CNode* NewNode = new CNode(NewPos, PopNode, m_EndPos);
				m_OpenList.push_back(NewNode);
				m_pMap->Change_Tile(NewPos, NODE);
			}
		}

		//오픈 리스트 정렬
		m_OpenList.sort(LessF);

		m_pMap->Update();
		Sleep(10);
	}
}
