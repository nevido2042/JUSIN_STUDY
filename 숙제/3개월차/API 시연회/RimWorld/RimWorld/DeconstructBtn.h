#pragma once
#include "Button.h"
class CDeconstructBtn :
    public CButton
{
public:
    CDeconstructBtn();
    virtual ~CDeconstructBtn();
public:
    void On_Click() override;
public:
    void Initialize() override;
    void Render(HDC hDC) override;
};

