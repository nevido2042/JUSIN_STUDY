#pragma once
#include "pch.h"
#include "Obj.h"
#include "Item.h"
#include "Player.h"

class CShop :
    public CObj
{
    CPlayer* m_pPlayer;
    vector<CItem*> m_vecItem;
public:
    CShop();
    virtual ~CShop();
public:
    void Initialize() override;
    void Update() override;
    void Release() override;
    void Render() override;
public:
    void Set_Player(CPlayer* _pPlayer)
    {
        m_pPlayer = _pPlayer;
    }
    void OpenShop();
    void Open_Buy_Menu();
    void Open_Sell_Menu();
};

