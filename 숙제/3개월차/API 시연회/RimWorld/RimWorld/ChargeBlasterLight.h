#pragma once
#include "RangedWeapon.h"
#include "Charge_Small.h"

class CChargeBlasterLight :
    public CRangedWeapon
{
public:
    CChargeBlasterLight();
    virtual ~CChargeBlasterLight();
public:
    void Fire()  override;
public:
    void Render(HDC hDC) override;
};

