#pragma once
#include "Obj.h"

#include "RimSlot.h"

class CRimSlotMgr :
    public CObj
{
public:
    CRimSlotMgr();
    virtual ~CRimSlotMgr();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

