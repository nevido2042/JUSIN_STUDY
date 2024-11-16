#pragma once
#include "Obj.h"
#include "EasySword.h"
#include "NormalSword.h"
#include "HardSword.h"

class CItemFactory :
    public CObj
{
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Relase() override;
    void Render() override;
public:
    CItem* Create_Item(const char* _szName);
};

