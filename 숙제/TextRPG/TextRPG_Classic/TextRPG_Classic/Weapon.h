#pragma once
#include "Equipment.h"
class CWeapon abstract:
    public CEquipment
{
    // CEquipment��(��) ���� ��ӵ�
public:
    void Use() override;
    void Initialize() override;
};

