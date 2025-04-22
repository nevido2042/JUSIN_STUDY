#pragma once
#include "Item.h"
class FireRate_Item :
    public Item
{
    virtual void Upgrade_Target() override;
    virtual void Render(HDC _hDC) override;
};

