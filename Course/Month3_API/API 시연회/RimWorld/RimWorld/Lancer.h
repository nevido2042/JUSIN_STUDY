#pragma once
#include "Enemy.h"
#include "ChargeLance.h"

class CLancer :
    public CEnemy
{
public:
    CLancer();
    virtual ~CLancer();
public:
    void    Initialize() override;
    int     Update() override;
    void    Late_Update() override;
    void    Render(HDC hDC) override;
    void    Dead() override;
};

