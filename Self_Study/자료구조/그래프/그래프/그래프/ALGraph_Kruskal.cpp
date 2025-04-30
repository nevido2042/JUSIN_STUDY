#include "pch.h"
#include "ALGraph_Kruskal.h"

void CALGraph_Kruskal::Initialize(int _iNumV)
{
	CALGraph_Search::Initialize(_iNumV);

}

void CALGraph_Kruskal::AddEdge(int _iFrom_V, int _iTo_V, int _iWeight)
{
	CALEdge Edge = { _iFrom_V, _iTo_V, _iWeight };
	m_vecADJList[_iFrom_V].push_back(_iTo_V);
	m_vecADJList[_iTo_V].push_back(_iFrom_V);
	++m_iNumE;

	m_PQ.push(Edge);
}

void CALGraph_Kruskal::Construct_KruskalMST()
{
	CALEdge RecvEdge[20];//������ ������ ����
	CALEdge Edge;
	int iEdgeIdx(0);

	while (m_iNumE + 1 > m_iNumV)
	{
		Edge = m_PQ.top();
		m_PQ.pop();
		Remove_Edge(Edge.m_iV1, Edge.m_iV2);

		if (!IsConnect_Vertex(Edge.m_iV1, Edge.m_iV2))
		{
			//���� ����
			RecoverEdge(Edge.m_iV1, Edge.m_iV2, Edge.m_weight);
			
			//������ ������ ������ ������ �����Ѵ�.
			RecvEdge[iEdgeIdx++] = Edge;
		}

	}

	//�켱���� ť���� ������ ������ ������ ȸ��
	for (int i = 0; i < iEdgeIdx; ++i)
	{
		m_PQ.push(RecvEdge[i]);
	}
}

void CALGraph_Kruskal::Show_GraphEdgeWeightInfo()
{
	priority_queue<CALEdge, vector<CALEdge>, CPQ_WeightCMP> CopyPQ = m_PQ;
	CALEdge Edge;
	while (!CopyPQ.empty())
	{
		Edge = CopyPQ.top();
		CopyPQ.pop();
		cout << char(Edge.m_iV1 + 65) << "-" << char(Edge.m_iV2 + 65) << " w: " << Edge.m_weight << endl;
	}
}

void CALGraph_Kruskal::Remove_Edge(int _iFrom_V, int _iTo_V)
{
	Remove_WayEdge(_iFrom_V, _iTo_V);
	Remove_WayEdge(_iTo_V, _iFrom_V);

	--m_iNumE;
}

void CALGraph_Kruskal::Remove_WayEdge(int _iFrom_V, int _iTo_V)
{
	int Edge;

	auto Iter = m_vecADJList[_iFrom_V].begin();
	while (Iter != m_vecADJList[_iFrom_V].end())
	{
		if (*Iter == _iTo_V)
		{
			m_vecADJList[_iFrom_V].erase(Iter);
			return;
		}
		++Iter;
	}
}

void CALGraph_Kruskal::RecoverEdge(int _iFrom_V, int _iTo_V, int _iWeight)
{
	m_vecADJList[_iFrom_V].push_back(_iTo_V);
	m_vecADJList[_iTo_V].push_back(_iFrom_V);
	++m_iNumE;
}

bool CALGraph_Kruskal::IsConnect_Vertex(int _iV1, int _iV2)
{
	stack<int> Stack;
	int iVisit_V(-1);

	//���� ��带 ���ÿ� �ִ´�.
	Stack.push(_iV1);

	while (!Stack.empty())
	{
		//������ ������� �ʴٸ� �ϳ� ������.
		iVisit_V = Stack.top();
		Stack.pop();

		//���� ��尡 ã�� ����ΰ�?
		if (iVisit_V == _iV2)
		{
			memset(m_pVisitInfo, false, sizeof(bool) * m_iNumV);
			return true;
		}

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
	return false;
}
