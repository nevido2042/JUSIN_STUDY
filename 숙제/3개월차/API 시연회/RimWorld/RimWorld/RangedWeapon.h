#pragma once
#include "Obj.h"
class CRangedWeapon abstract:
    public CObj
{
public:
    CRangedWeapon();
    virtual ~CRangedWeapon();

public:
    virtual void    Fire();
protected:
    bool            Try_Fire();

public:
    float           Get_Range() { return m_fRange; }
protected:
    void            Follow_Pawn();
public:
    void            Initialize() override;
    int             Update() override;
    void            Late_Update() override;
    virtual void    Render(HDC hDC) PURE;
    void            Release() override;

protected:
    //발사 속도(프레임)
    int   m_iFireRate;
    //마지막 발사 시간(프레임)
    int   m_iLastFireTime;
    //발사체
    CObj*       m_pProjectile;
    //무기 사정거리
    float       m_fRange;
};

