#pragma once
#include "Equipment.h"
class CArmor abstract :
    public CEquipment
{
    // CEquipment��(��) ���� ��ӵ�
public:
    void Initialize() = 0;
};

