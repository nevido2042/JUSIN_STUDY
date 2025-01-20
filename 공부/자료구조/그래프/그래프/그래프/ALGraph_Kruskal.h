#pragma once
#include "ALGraph_Search.h"
#include <queue>
#include "ALEdge.h"

class CALGraph_Kruskal :
    public CALGraph_Search
{
public:
    virtual void Initialize(int _iNumV) override;
    void AddEdge(int _iFrom_V, int _iTo_V, int _iWeight);
    void Construct_KruskalMST();
    void Show_GraphEdgeWeightInfo();
    void Remove_Edge(int _iFrom_V, int _iTo_V);
    void Remove_WayEdge(int _iFrom_V, int _iTo_V);
    void RecoverEdge(int _iFrom_V, int _iTo_V, int _iWeight);
    bool IsConnect_Vertex(int _iV1, int _iV2);
private:
    class CPQ_WeightCMP 
    {
    public:
        bool operator()(CALEdge& _Edge1, CALEdge& _Edge2) const
        {
            return _Edge1.m_weight < _Edge2.m_weight;
        }
    };

    priority_queue<CALEdge, vector<CALEdge>, CPQ_WeightCMP> m_PQ;
};

