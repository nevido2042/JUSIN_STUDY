#pragma once
#include "Pawn.h"
class CCentipede :
    public CPawn
{
public:
    CCentipede();
    virtual ~CCentipede();
public:
    void Initialize() override;
    void Render(HDC hDC) override;
};

