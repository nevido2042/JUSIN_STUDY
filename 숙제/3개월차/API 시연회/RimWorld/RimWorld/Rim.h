#pragma once
#include "Obj.h"
class CRim :
    public CObj
{
public:
    CRim();
    virtual ~CRim();
public:
    void Move_To(float _fX, float _fY);

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

