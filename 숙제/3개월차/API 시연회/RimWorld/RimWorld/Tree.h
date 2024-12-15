#pragma once
#include "Obj.h"
class CTree :
    public CObj
{
public:
    CTree();
    virtual ~CTree();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

