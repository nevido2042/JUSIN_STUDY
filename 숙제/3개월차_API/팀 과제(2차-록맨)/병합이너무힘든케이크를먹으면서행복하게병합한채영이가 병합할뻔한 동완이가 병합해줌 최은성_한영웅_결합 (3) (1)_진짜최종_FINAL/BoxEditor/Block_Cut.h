#pragma once
#include "Block.h"
class CBlock_Cut :
    public CBlock
{
    void Initialize();
    void Render(HDC hDC);
};

