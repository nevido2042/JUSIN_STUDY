#pragma once
#include "Obj.h"
class CShip :
    public CObj
{
public:
    CShip();
    virtual ~CShip();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

