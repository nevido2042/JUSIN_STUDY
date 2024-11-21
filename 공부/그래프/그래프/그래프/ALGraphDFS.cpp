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
	int iNext_V;
	auto Iter_Next_V = m_vecADJList[iVisit_V].begin();

	Visit_Vertex(iVisit_V);
	Stack.push(iVisit_V);

	while (m_vecADJList[iVisit_V].size() != 0)
	{
		Iter_Next_V = m_vecADJList[iVisit_V].begin();

		bool bVisitFlag = false;

		if (Visit_Vertex(*Iter_Next_V))
		{
			Stack.push(iVisit_V);
			iVisit_V = *Iter_Next_V;
			bVisitFlag = true;
		}
		else
		{
			++Iter_Next_V;
			
			while (m_vecADJList[*Iter_Next_V].size() != 0)
			{
				if (Visit_Vertex(*Iter_Next_V))
				{
					Stack.push(iVisit_V);
					bVisitFlag = true;
					break;
				}
			}
		}

		if (bVisitFlag == false)
		{
			if (Stack.empty())
			{
				break;
			}
			else
			{
				iVisit_V = Stack.top();
				Stack.pop();
			}
		}
	}
	memset(m_pVisitInfo, 0, sizeof(bool) * m_iNumV);
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
