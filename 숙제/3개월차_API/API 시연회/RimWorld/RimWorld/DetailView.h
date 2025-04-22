#pragma once
#include "Obj.h"
class CDetailView :
    public CObj
{
public:
    CDetailView();
    virtual ~CDetailView();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Set_Activate(bool _bActivate) override;
};

