#pragma once
#include "Pawn.h"
#include "Inventory.h"

class CPlayer :
    public CPawn
{
    CInventory* m_pInventory;
public:
    CPlayer();
    virtual ~CPlayer();
    
public:

    void Initialize() override;

    void Update() override;

    void Relase() override;

public:
    CInventory* Get_Inventory()
    {
        return m_pInventory;
    }

};

