#pragma once
#include "pch.h"
#include "Obj.h"
#include "Item.h"

class CShop :
    public CObj
{
    vector<CItem*> m_vecItem;
public:
    CShop();
    virtual ~CShop();
public:
    void Initialize() override;
    void Update() override;
    void Relase() override;
    void Render() override;
};

