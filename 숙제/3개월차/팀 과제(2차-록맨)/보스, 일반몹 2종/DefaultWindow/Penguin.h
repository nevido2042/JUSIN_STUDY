#pragma once
#include "CMonster.h"
class CPenguin :
    public CMonster
{
public:
    virtual void Initialize() override;
    virtual void Render(HDC hDC) override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Release() override;
};

