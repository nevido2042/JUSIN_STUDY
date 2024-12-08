#pragma once
#include "CBullet.h"
class CIce_Slasher :
    public CBullet
{
    void Initialize() override;
    void Render(HDC hDC) override;
    void OnCollision(CObj* _pOther, OBJID _eOtherID) override;

    // CBullet을(를) 통해 상속됨
    int Update() override;
    void Late_Update() override;
    void Release() override;
};

