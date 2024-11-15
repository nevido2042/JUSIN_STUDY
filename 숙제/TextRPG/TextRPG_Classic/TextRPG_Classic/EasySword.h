#pragma once
#include "Equipment.h"
class CEasySword :
    public CEquipment
{
public:
    // CEquipment을(를) 통해 상속됨
    void Initialize() override;
};

