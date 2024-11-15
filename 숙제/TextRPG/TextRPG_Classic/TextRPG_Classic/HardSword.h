#pragma once
#include "Equipment.h"
class CHardSword :
    public CEquipment
{
    // CEquipment을(를) 통해 상속됨
    void Initialize() override;
};

