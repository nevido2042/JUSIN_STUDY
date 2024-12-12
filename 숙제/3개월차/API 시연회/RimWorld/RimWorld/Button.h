#pragma once
#include "Obj.h"
class CButton abstract:
    public CObj
{
public:
    CButton();
    virtual ~CButton();

public:
    void Initialize() PURE;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC);
    void Release() override;
};

