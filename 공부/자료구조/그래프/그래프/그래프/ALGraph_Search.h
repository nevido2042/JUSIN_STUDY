#pragma once
#include "ALGraph.h"
class CALGraph_Search :
    public CALGraph
{
public:
    ~CALGraph_Search();
public:
    virtual void Initialize(int _iNumV) override;
    virtual void Release() override;
    void Show_DFS_GraphVertex(int _iStartV);
    void Show_BFS_GraphVertex(int _iStartV);
    bool Visit_Vertex(int _iVisitV);
public:
    bool* m_pVisitInfo;
};

