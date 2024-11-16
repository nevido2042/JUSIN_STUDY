#pragma once
#include "Obj.h"
#include "ItemList.h"

class CItemFactory :
    public CObj
{
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Release() override;
    void Render() override;
public:
    CItem* Create_Item(const char* _szName);
};

