#pragma once
#include "CBullet.h"
class CIce_Slasher :
    public CBullet
{
    void Initialize() override;
    void Render(HDC hDC) override;
};

