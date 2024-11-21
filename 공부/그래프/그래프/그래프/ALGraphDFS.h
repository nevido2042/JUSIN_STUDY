#pragma once
#include "ALGraph.h"
class CALGraphDFS :
    public CALGraph
{
public:
    ~CALGraphDFS();
public:
    virtual void Initialize(int _iNumV) override;
    virtual void Release() override;
    void Show_DFS_GraphVertex(int _iStartV);
    bool Visit_Vertex(int _iVisitV);
public:
    bool* m_pVisitInfo;

};

