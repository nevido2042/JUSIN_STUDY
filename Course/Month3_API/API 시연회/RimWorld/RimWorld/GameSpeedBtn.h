#pragma once
#include "Obj.h"
class CGameSpeedBtn :
    public CObj
{
public:
    CGameSpeedBtn();
    virtual ~CGameSpeedBtn();
public:
    void Initialize()	override;
    void Late_Update()	override;
    void Render(HDC hDC) override;
    void Release()		override;
    int Update() override;
};

