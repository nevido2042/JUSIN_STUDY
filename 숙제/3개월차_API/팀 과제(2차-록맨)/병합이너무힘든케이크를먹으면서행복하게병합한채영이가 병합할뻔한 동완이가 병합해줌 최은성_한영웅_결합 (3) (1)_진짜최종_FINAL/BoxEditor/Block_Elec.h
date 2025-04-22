#pragma once
#include "Block.h"
class CBlock_Elec :
    public CBlock
{
    void Initialize();
    void Render(HDC hDC);
};

