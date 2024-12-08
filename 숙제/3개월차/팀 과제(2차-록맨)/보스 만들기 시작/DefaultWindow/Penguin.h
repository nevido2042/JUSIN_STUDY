#pragma once
#include "CMonster.h"
class CPenguin :
    public CMonster
{
    virtual void Initialize() override;
    virtual void Render(HDC hDC) override;
};

