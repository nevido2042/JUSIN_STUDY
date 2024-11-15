#pragma once
#include "Obj.h"
class CItem abstract:
    public CObj
{
    // CObj을(를) 통해 상속됨
public:
    void Initialize() = 0;
    void Update() override;
    void Relase() override;
    void Render() override;
};

