#include "pch.h"
#include "ALGraph_Search.h"
#include <stack>
#include <queue>

CALGraph_Search::~CALGraph_Search()
{
	Release();
}

void CALGraph_Search::Initialize(int _iNumV)
{
	CALGraph::Initialize(_iNumV);
	m_pVisitInfo = new bool[_iNumV];
	memset(m_pVisitInfo, false, sizeof(bool) * m_iNumV);
}

void CALGraph_Search::Release()
{
	CALGraph::Release();

	if (m_pVisitInfo)
	{
		delete m_pVisitInfo;
		m_pVisitInfo = nullptr;
	}
}

void CALGraph_Search::Show_DFS_GraphVertex(int _iStartV)
{
	stack<int> Stack;
	int iVisit_V(-1);
	
	//시작 노드를 스택에 넣는다.
	Stack.push(_iStartV);

	while (!Stack.empty())
	{
		//스택이 비어있지 않다면 하나 꺼낸다.
		iVisit_V = Stack.top();
		Stack.pop();
		//꺼낸 노드를 방문 시도 한다.
		if (!Visit_Vertex(iVisit_V))
		{
			//방문에 실패했으면 다시
			continue;
		}


		//방문한 노드의 연결된 지점을 모두 스택에 넣는다.
		auto Iter = m_vecADJList[iVisit_V].begin();
		while (Iter != m_vecADJList[iVisit_V].end())
		{
			Stack.push(*Iter);
			++Iter;
		}
	}

	memset(m_pVisitInfo, false, sizeof(bool) * m_iNumV);
}

void CALGraph_Search::Show_BFS_GraphVertex(int _iStartV)
{
	queue<int> Stack;
	int iVisit_V(-1);

	//시작 노드를 스택에 넣는다.
	Stack.push(_iStartV);

	while (!Stack.empty())
	{
		//스택이 비어있지 않다면 하나 꺼낸다.
		iVisit_V = Stack.front();
		Stack.pop();
		//꺼낸 노드를 방문 시도 한다.
		if (!Visit_Vertex(iVisit_V))
		{
			//방문에 실패했으면 다시
			continue;
		}


		//방문한 노드의 연결된 지점을 모두 스택에 넣는다.
		auto Iter = m_vecADJList[iVisit_V].begin();
		while (Iter != m_vecADJList[iVisit_V].end())
		{
			Stack.push(*Iter);
			++Iter;
		}
	}

	memset(m_pVisitInfo, false, sizeof(bool) * m_iNumV);
}

bool CALGraph_Search::Visit_Vertex(int _iVisitV)
{
	if (m_pVisitInfo[_iVisitV] == false)
	{
		m_pVisitInfo[_iVisitV] = true;
		cout << char(_iVisitV + 65);
		return true;
	}
	return false;
}
