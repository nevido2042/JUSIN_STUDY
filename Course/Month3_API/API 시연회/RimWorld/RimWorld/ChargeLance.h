#pragma once
#include "RangedWeapon.h"
#include "Charge_Small.h"

class CChargeLance :
    public CRangedWeapon
{
public:
    CChargeLance();
    virtual ~CChargeLance();
public:
    void Initialize() override;
    void Fire()  override;
public:
    void Render(HDC hDC) override;
};

