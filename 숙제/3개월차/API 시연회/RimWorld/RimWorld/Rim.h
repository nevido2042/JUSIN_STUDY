#pragma once
#include "Pawn.h"
#include "Node.h"

#include "BoltActionRifle.h"

class CRim :
    public CPawn
{
public:
    enum STATE { DRAFTED, UNDRAFTED, WORKING, END };
public:
    CRim();
    virtual ~CRim();

public:
    void Initialize() override;
    int  Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;

private:
    STATE m_eState;
};
