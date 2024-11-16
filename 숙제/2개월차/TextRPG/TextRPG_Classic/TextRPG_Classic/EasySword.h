#pragma once
#include "Weapon.h"

class CEasySword :
    public CWeapon
{
public:
    // CEquipment을(를) 통해 상속됨
    void Initialize() override;
};

