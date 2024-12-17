#pragma once
#include "Button.h"
class CSteelWallBtn :
    public CButton
{
public:
    CSteelWallBtn();
    virtual ~CSteelWallBtn();
public:
    void On_Click() override;
public:
    void Initialize() override;
    void Render(HDC hDC) override;
};

