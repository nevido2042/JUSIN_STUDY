#pragma once
#include "Pawn.h"
class CBoomrat :
    public CPawn
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
    void Handle_Wandering() override;
    void Handle_Chasing() override;
    void Handle_Deconstructing() override;
private:
    void    Find_Target();
    void    Boom();
};

