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

void CPathFinder::Start()
{
	CNode* StartNode = new CNode(m_StartPos);
	m_OpenList.push_back(StartNode);
	
	while (!m_OpenList.empty())
	{
		//�ϳ� �̴´�.
		CNode* PopNode = m_OpenList.front();
		m_OpenList.pop_front();

		//���� �������� Ȯ���Ѵ�.
		if (m_EndPos == PopNode->Get_Pos())
		{
			system("pause");
			return;
		}

		//Ŭ���� ����Ʈ�� �ִ´�.
		m_CloseList.push_back(PopNode);

		//8���� Ž���Ѵ�.
		int iDX[8]{ 0, 1, 1, 1,	0, -1, -1, -1 };
		int iDY[8]{	-1,	-1, 1, 1, 1, 1,	0, -1 };

		for (int i = 0; i < 8; ++i)
		{
			int iX = PopNode->Get_Pos().iX + iDX[i];
			int iY = PopNode->Get_Pos().iY + iDY[i];

			//�� ���ΰ�?
			if (iX < 0 || iY < 0 || m_pMap->Get_Width() - 1 < iX || m_pMap->Get_Height() - 1 < iY)
			{
				continue;
			}

			POS NewPos(iX, iY);

			//���ΰ�?
			if (m_pMap->Get_Tile_Type(NewPos) == WALL)
			{
				continue;
			}

			//Ŭ���� ����Ʈ �Ǵ� ���¸���Ʈ�� ���� �ϴ°�?
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
				m_OpenList.push_back(new CNode(NewPos));
				m_pMap->Change_Tile(NewPos, NODE);
			}
		}

		//���� ����Ʈ ����
		m_OpenList.sort();

		m_pMap->Update();
		Sleep(10);
	}
}
