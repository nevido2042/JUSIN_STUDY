#pragma once
#include "Breakable.h"
class CTree :
    public CBreakable
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

