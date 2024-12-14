#pragma once
#include "Obj.h"
#include "Node.h"

#include "HealthBar.h"

class CPawn :
    public CObj
{
public:
    CPawn();
    virtual ~CPawn();
public:
    void Move_To(POS _Pos);

public:
    float Get_HP() { return m_fHP; }
    float Get_MaxHP() { return m_fMaxHP; }

protected:
    void Navigate();
    void Calculate_MoveDir();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) PURE;
    void Release() override;

protected:
    //길찾기 관련
    list<CNode*>    m_NodeList;
    bool            m_bNavigating;
    //방향 결정 용(이전 프레임 위치)
    POS             m_tPrevPos;
    //체력 관련
    float           m_fHP;
    float           m_fMaxHP;
};

