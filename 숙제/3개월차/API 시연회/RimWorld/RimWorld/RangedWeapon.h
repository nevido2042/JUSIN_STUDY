#pragma once
#include "Obj.h"
class CRangedWeapon abstract:
    public CObj
{
public:
    CRangedWeapon();
    virtual ~CRangedWeapon();

protected:
    void Follow_Pawn();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) PURE;
    void Release() override;

protected:
    ULONGLONG   m_ullFireRate;
    CObj*       m_pProjectile;
};

