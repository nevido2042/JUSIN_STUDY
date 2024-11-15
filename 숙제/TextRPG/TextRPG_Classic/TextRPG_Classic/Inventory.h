#pragma once
#include "pch.h"
#include "Obj.h"
#include "Item.h"

class CInventory :
    public CObj
{
    int m_iMaxSize;
    vector<CItem*> m_vecItem;
public:
    CInventory();
    virtual ~CInventory();
public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Relase() override;
    void Render() override;
public:
    void Add_Item(CItem* _pItem);
    //void Remove_Item();
};

