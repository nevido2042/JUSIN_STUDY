#include "pch.h"
#include "ALGraphDFS.h"
#include <stack>

CALGraphDFS::~CALGraphDFS()
{
	Release();
}

void CALGraphDFS::Initialize(int _iNumV)
{
	CALGraph::Initialize(_iNumV);
	m_pVisitInfo = new bool[_iNumV];
	memset(m_pVisitInfo, false, sizeof(bool) * m_iNumV);
}

void CALGraphDFS::Release()
{
	CALGraph::Release();

	if (m_pVisitInfo)
	{
		delete m_pVisitInfo;
		m_pVisitInfo = nullptr;
	}
}

void CALGraphDFS::Show_DFS_GraphVertex(int _iStartV)
{
	stack<int> Stack;
	int iVisit_V = _iStartV;
	int iPrevVisit_V(-1);

	while (true)
	{
		if (Visit_Vertex(iVisit_V)) // 방문 성공
		{
			iPrevVisit_V = iVisit_V;
			Stack.push(iVisit_V);//방문 순서 저장
			iVisit_V = *m_vecADJList[iVisit_V].begin();//방문한 노드와 연결된 녀석을 다음 탐색으로 지정
		}
		else
		{
			//실패했으면 다른 인접노드도 검사
			//int iStackTop = Stack.top();
			auto Iter = m_vecADJList[iPrevVisit_V].begin();

			bool bVisitFlag = false;
			while (Iter != m_vecADJList[iPrevVisit_V].end())
			{
				iVisit_V = *Iter;
				if (Visit_Vertex(iVisit_V)) // 방문 성공
				{
					iPrevVisit_V = iVisit_V;
					Stack.push(iVisit_V);//방문 순서 저장
					iVisit_V = *m_vecADJList[iVisit_V].begin();//방문한 노드와 연결된 녀석을 다음 탐색으로 지정
					bVisitFlag = true;
					break;
				}
				else
				{
					++Iter;
				}
				
			}

			if (Stack.empty())
			{
				memset(m_pVisitInfo, 0, sizeof(bool) * m_iNumV);
				break;
			}

			if (bVisitFlag == false)
			{
				iVisit_V = Stack.top();
				iPrevVisit_V = Stack.top();
				Stack.pop();
			}
			
		}
		

	}
	
	
}

bool CALGraphDFS::Visit_Vertex(int _iVisitV)
{
	if (m_pVisitInfo[_iVisitV] == false)
	{
		m_pVisitInfo[_iVisitV] = true;
		cout << char(_iVisitV + 65);
		return true;
	}
	return false;
}
