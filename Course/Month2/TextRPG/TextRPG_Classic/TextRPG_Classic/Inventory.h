#pragma once
#include "pch.h"
#include "Obj.h"
#include "Item.h"
#include "Pawn.h"

class CInventory :
    public CObj
{
    int m_iMaxSize;
    vector<CItem*> m_vecItem;
    CPawn* m_pOwner;
public:
    CInventory();
    virtual ~CInventory();
public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Release() override;
    void Render() override;
public:
    void Set_Owner(CPawn* _pPawn)
    {
        m_pOwner = _pPawn;
    }
    void Add_Item(CItem* _pItem);
    void Use_Item(int _iIndex);
    bool Is_Full()
    {
        if (m_iMaxSize == m_vecItem.size())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    size_t Get_ItemCount()
    {
        return m_vecItem.size();
    }
    void Remove_Item(int _iIndex);
    void Pop_Item(int _iIndex);
    int Get_MaxSize()
    {
        return m_iMaxSize;
    }
};

