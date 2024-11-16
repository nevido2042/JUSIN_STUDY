#pragma once
#include "Obj.h"
#include "EasySword.h"
#include "NormalSword.h"
#include "HardSword.h"

class CItemFactory :
    public CObj
{
    // CObj��(��) ���� ��ӵ�
    void Initialize() override;
    void Update() override;
    void Release() override;
    void Render() override;
public:
    CItem* Create_Item(const char* _szName);
};

