#pragma once
#include "Enemy.h"

#include "ChargeBlasterLight.h"

class CCentipede :
    public CEnemy
{
public:
    CCentipede();
    virtual ~CCentipede();
public:
    void    Initialize() override;
    int     Update() override;
    void    Late_Update() override;
    void    Render(HDC hDC) override;
    void    Dead() override;
};

