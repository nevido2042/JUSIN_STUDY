#pragma once
#include "Pawn.h"

#include "ChargeBlasterLight.h"

class CCentipede :
    public CPawn
{
public:
    CCentipede();
    virtual ~CCentipede();
public:
    void    Initialize() override;
    int     Update() override;
    void    Late_Update() override;
    void    Render(HDC hDC) override;
private:
    void    Find_Target();

protected:
    void Dead() override;
};

