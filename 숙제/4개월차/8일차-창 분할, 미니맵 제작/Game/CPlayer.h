#pragma once
#include "CObj.h"
class CPlayer :
    public CObj
{
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Render() override;
    void Release() override;
};

