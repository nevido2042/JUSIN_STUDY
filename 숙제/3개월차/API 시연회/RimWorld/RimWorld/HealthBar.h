#pragma once
#include "Obj.h"
class CHealthBar :
    public CObj
{
public:
    CHealthBar();
    virtual ~CHealthBar();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
private:
    void Follow_Pawn();
};

