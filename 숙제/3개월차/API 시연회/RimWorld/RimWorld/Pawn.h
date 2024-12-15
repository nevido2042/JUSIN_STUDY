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
    void Take_Damage(float _fDamage);
public:
    float Get_HP() { return m_fHP; }
    float Get_MaxHP() { return m_fMaxHP; }
    float Get_IsDead() { return m_bDead; }
    float Get_TargetAngle() { return m_fTargetAngle; }
protected:
    virtual void Dead();//죽었을 때 할 행동

protected:
    void    Navigate();
    void    Calculate_MoveDir();
    void    Measure_Target();
    bool    IsWithinRange();//사정거리에 있는가?
    bool    IsCanSeeTarget();//상대가 보이는가?(타겟과 자신사이에 장애물이 있으면 안보이는 것)

public:
    void    Initialize() override;
    int     Update() override;
    void    Late_Update() override;
    void    Render(HDC hDC) PURE;
    void    Release() override;
    void    OnCollision(OBJID _eID, CObj* _pOther) override;

protected:
    //길찾기 관련
    list<CNode*>    m_NodeList;
    bool            m_bNavigating;
    //방향 결정 용(이전 프레임 위치)
    POS             m_tPrevPos;
    //체력 관련
    float           m_fHP;
    float           m_fMaxHP;
    bool            m_bDead;
    //원거리 무기
    CObj*           m_pRangedWeapon;
    
    //타겟과의 거리
    float           m_fTargetDist;
    //타겟과의 각도
    float           m_fTargetAngle;
    //공격 중 인가?
    bool            m_bAttack;
};

