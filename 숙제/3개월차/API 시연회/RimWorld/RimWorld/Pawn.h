#pragma once
#include "Obj.h"
#include "Node.h"

class CPawn :
    public CObj
{
public:
    CPawn();
    virtual ~CPawn();
public:
    void Move_To(POS _Pos);

protected:
    void Navigate();
    void Calculate_MoveDir();
public:
    void Initialize() PURE;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) PURE;
    void Release() override;

protected:
    list<CNode*>    m_NodeList;
    bool            m_bNavigating;
    POS             m_tPrevPos;
};

