#pragma once
#include "RangedWeapon.h"
class CChargeBlasterLight :
    public CRangedWeapon
{
public:
    CChargeBlasterLight();
    virtual ~CChargeBlasterLight();
public:
    void Render(HDC hDC) override;
};

