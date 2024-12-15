#pragma once
#include "RangedWeapon.h"

#include "Bullet_Small.h"

class CBoltActionRifle :
    public CRangedWeapon
{
public:
    CBoltActionRifle();
    virtual ~CBoltActionRifle();
public:
    void Fire()  override;
public:
    void Render(HDC hDC) override;
};

