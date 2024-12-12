#pragma once
#include "Obj.h"
class CRock :
    public CObj
{
public:
    enum STATE { IDLE, WALK, ATTACK, HIT, DEAD, END };
public:
    CRock();
    virtual ~CRock();
public:
    void    Change_Image();
public:
    void    Initialize() override;
    int     Update() override;
    void    Late_Update() override;
    void    Render(HDC hDC) override;
    void    Release() override;

protected:
    STATE               m_eCurState;
    STATE               m_ePreState;
};

