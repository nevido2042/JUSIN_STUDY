#include "pch.h"
#include "ALGraph.h"

void CALGraph::Initialize(int _iNumV)
{
	m_vecADJList.reserve(_iNumV);
	for (int i = 0; i < _iNumV; ++i)
	{
		m_vecADJList.push_back(list<int>());
	}
	m_iNumV = _iNumV;
	m_iNumE = 0;
}

void CALGraph::Release()
{
}

void CALGraph::AddEdge(int _iFromV, int _iToV)
{
	m_vecADJList[_iFromV].push_back(_iToV);
	m_vecADJList[_iToV].push_back(_iFromV);

	m_vecADJList[_iFromV].sort();
	m_vecADJList[_iToV].sort();

	++m_iNumE;
}

void CALGraph::Show_GraphEdgeInfo()
{
	for (int i = 0; i < m_iNumV; ++i)
	{
		cout << char(i + 65) << "와 연결된 정점: ";
		auto Iter = m_vecADJList[i].begin();
		while (Iter != m_vecADJList[i].end())
		{
			cout << char(*Iter + 65);
			++Iter;
		}
		cout << endl;
	}
}
