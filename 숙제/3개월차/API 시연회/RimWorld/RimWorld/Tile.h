#pragma once
#include "Obj.h"
//타일은 타일매니저를 통해서만 생성하자
class CTile :
    public CObj
{
public:
    CTile();
    virtual ~CTile();
public:
    void Initialize() override;
    int Update() override;
    int Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

