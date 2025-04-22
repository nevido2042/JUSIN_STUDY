#pragma once
#include "Block.h"
class CBlock_Ice :
    public CBlock
{
    void Initialize() override;
    void Render(HDC hDC) override;
};

