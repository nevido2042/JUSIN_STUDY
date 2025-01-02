#pragma once
#include "Enemy.h"
class CBoomrat :
    public CEnemy
{
public:
    CBoomrat();
    virtual ~CBoomrat();
public:
    void    Initialize() override;
    int     Update() override;
    void    Late_Update() override;
    void    Render(HDC hDC) override;
    void    Dead() override;
protected:
    void Handle_Chasing() override;
    void    Boom();
};

