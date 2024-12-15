#pragma once
#include "Button.h"
class CGameSpeedBtn :
    public CButton
{
public:
    CGameSpeedBtn();
    virtual ~CGameSpeedBtn();
public:
    void Initialize()	override;
    void Late_Update()	override;
    void Render(HDC hDC) override;
    void Release()		override;
};

