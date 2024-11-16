#pragma once
#include "Weapon.h"

class CNormalSword :
    public CWeapon
{
    // CEquipment을(를) 통해 상속됨
    void Initialize() override;
};

