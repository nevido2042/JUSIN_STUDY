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
    void            Follow_Pawn();
public:
    void            Initialize() override;
    int             Update() override;
    void            Late_Update() override;
    virtual void    Render(HDC hDC) PURE;
    void            Release() override;

protected:
    ULONGLONG   m_ullFireRate;
    ULONGLONG   m_ullLastFireTime;
    CObj*       m_pProjectile;
};

