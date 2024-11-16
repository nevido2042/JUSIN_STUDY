#pragma once
#include "Equipment.h"
class CWeapon abstract:
    public CEquipment
{
    // CEquipment을(를) 통해 상속됨
public:
    void Use() override;
    void Initialize() override;
};

