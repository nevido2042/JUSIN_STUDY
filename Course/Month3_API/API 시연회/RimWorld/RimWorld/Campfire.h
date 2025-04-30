#pragma once
#include "Obj.h"
class CCampfire :
    public CObj
{
public:
    CCampfire();
    virtual ~CCampfire();
public:
    void Initialize() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

