#pragma once
#include "Equipment.h"
class CArmor abstract :
    public CEquipment
{
    // CEquipment을(를) 통해 상속됨
public:
    void Initialize() = 0;
};

