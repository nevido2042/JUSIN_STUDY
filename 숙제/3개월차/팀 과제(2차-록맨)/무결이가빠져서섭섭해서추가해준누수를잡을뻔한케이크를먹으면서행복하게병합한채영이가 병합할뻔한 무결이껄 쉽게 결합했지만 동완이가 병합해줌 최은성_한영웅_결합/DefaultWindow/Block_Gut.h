#pragma once
#include "Block.h"
class CBlock_Gut :
    public CBlock
{
    void Initialize();
    void Render(HDC hDC);
};

