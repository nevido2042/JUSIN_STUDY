#pragma once
#include "Obj.h"
class CRock :
    public CObj
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
    CRock();
    virtual ~CRock();
public:
    void    Set_CheckNeighbor(bool _b) { m_bCheckNeighbor = _b; }
    void    Change_Image();

public:
    void    Initialize() override;
    int     Update() override;
    void    Late_Update() override;
    void    Render(HDC hDC) override;
    void    Release() override;
private:
    void    Check_Neighbor();

protected:
    STATE               m_eCurState;
    STATE               m_ePreState;

private:
    int     m_iRenderX;
    int     m_iRenderY;

    bool m_bCheckNeighbor;
};

