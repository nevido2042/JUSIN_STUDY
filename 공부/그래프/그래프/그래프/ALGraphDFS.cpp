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
	int iVisit_V(-1);
	
	//���� ��带 ���ÿ� �ִ´�.
	Stack.push(_iStartV);

	while (!Stack.empty())
	{
		//������ ������� �ʴٸ� �ϳ� ������.
		iVisit_V = Stack.top();
		Stack.pop();
		//���� ��带 �湮 �õ� �Ѵ�.
		if (!Visit_Vertex(iVisit_V))
		{
			//�湮�� ���������� �ٽ�
			continue;
		}


		//�湮�� ����� ����� ������ ��� ���ÿ� �ִ´�.
		auto Iter = m_vecADJList[iVisit_V].begin();
		while (Iter != m_vecADJList[iVisit_V].end())
		{
			Stack.push(*Iter);
			++Iter;
		}
	}

	memset(m_pVisitInfo, false, sizeof(bool) * m_iNumV);
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
