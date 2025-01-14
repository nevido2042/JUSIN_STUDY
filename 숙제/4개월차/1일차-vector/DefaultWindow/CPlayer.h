#pragma once
#include "CObj.h"

class CPlayer :    public CObj
{
public:
    CPlayer();
    virtual ~CPlayer();
     
public:
    void Initialize() override;
    void Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    void        Key_Input();

};

