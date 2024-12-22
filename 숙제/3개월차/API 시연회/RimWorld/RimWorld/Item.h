#pragma once
#include "Obj.h"
class CItem :
    public CObj
{
public:
    CItem();
    virtual ~CItem();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

