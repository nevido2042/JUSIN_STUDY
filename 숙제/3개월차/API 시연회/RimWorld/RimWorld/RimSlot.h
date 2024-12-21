#pragma once
#include "Obj.h"
class CRimSlot :
    public CObj
{
public:
    CRimSlot();
    virtual ~CRimSlot();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

