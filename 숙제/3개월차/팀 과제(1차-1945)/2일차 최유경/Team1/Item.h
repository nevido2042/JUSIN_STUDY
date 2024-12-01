#pragma once
#include "Obj.h"
class Item :
    public Obj
{
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _hdc) override;
    void Release() override;

private:
    virtual void Upgrade_Target();
};

