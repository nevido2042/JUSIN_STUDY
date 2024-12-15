#pragma once
#include "Projectile.h"
class CBullet_Small :
    public CProjectile
{
public:
    CBullet_Small();
    virtual ~CBullet_Small();
public:
    void Render(HDC hDC) override;
};

