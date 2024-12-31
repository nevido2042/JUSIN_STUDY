#pragma once
#include "Obj.h"
class CPanel :
    public CObj
{   
public:
    CPanel();
    virtual ~CPanel();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

