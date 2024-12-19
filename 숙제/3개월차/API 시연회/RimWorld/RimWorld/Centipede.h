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
protected:
    void Handle_Wandering() override;
    void Handle_Chasing() override;
    void Handle_Deconstructing() override;
private:
    void    Find_Target();
};

