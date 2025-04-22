#pragma once
#include "Obj.h"
class CMsgBox :
    public CObj
{
public:
    CMsgBox();
    virtual ~CMsgBox();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

