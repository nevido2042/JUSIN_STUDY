#pragma once
#include "Pawn.h"
#include "ChargeLance.h"

class CLancer :
    public CPawn
{
public:
    CLancer();
    virtual ~CLancer();
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

