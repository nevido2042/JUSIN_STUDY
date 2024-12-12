#pragma once
#include "Obj.h"
#include "Node.h"

class CRim :
    public CObj
{
public:
    CRim();
    virtual ~CRim();
public:
    void Move_To(POS _Pos);

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
private:
    list<CNode*> m_NodeList;
};

