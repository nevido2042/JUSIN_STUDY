#pragma once
#include "Breakable.h"

class CSteelWall :
    public CBreakable
{
public:
    enum STATE 
    { 
        SOLO, HORIZONTAL, VERTICAL,
        END_LEFT, END_RIGHT,END_TOP, END_BOTTOM,
        RIGHT_BOTTOM, LEFT_BOTTOM, RIGHT_TOP, LEFT_TOP,
        T12, T3, T6, T9,
        CROSS,
        END 
    };
public:
    CSteelWall();
    virtual ~CSteelWall();
public:
    void    Change_Image();
public:
    void    Set_CheckNeighbor(bool _b) { m_bCheckNeighbor = _b; }
public:
    void    Initialize() override;
    int     Update() override;
    void    Late_Update() override;
    void    Render(HDC hDC) override;
    void    Release() override;
    void    OnCollision(OBJID _eID, CObj* _pOther) override;

private:
    void    Check_Neighbor();

    void    Set_State(STATE _eNewState);

protected:
    STATE               m_eCurState;
    STATE               m_ePreState;

private:
    int     m_iRenderX;
    int     m_iRenderY;

    bool    m_bCheckNeighbor;
};

