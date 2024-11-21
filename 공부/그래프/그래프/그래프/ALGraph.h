#pragma once
#include <list>
#include <vector>

enum { A, B, C, D, E, F, G, H, I, J };

class CALGraph
{
public:
	int m_iNumV;
	int m_iNumE;
	vector<list<int>> m_vecADJList;

	virtual void Initialize(int _iNumV);
	virtual void Release();
	void AddEdge(int _iFromV, int _iToV);
	void Show_GraphEdgeInfo();
};

