#pragma once
#include "Projectile.h"
class CCharge_Small :
    public CProjectile
{
public:
    CCharge_Small();
    virtual ~CCharge_Small();
public:
    void Initialize() override;
    void Render(HDC hDC) override;
};

