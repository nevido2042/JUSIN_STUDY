#pragma once
#include "Pawn.h"
class CEnemy :
    public CPawn
{
public:
    CEnemy();
    virtual ~CEnemy();
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

