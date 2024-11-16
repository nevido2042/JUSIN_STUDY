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
    int Get_HP()
    {
        return m_iHP;
    }
    int Get_STR()
    {
        return m_iSTR;
    }
};

