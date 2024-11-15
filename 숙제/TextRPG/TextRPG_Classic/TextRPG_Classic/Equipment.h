#pragma once
#include "Item.h"
class CEquipment abstract:
    public CItem
{
protected:
    int m_iHP;
    int m_iSTR;
public:
    CEquipment();
public:
    void Initialize() = 0;
    void Render() override;
};

