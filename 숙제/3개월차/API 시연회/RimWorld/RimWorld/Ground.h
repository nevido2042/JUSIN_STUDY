#pragma once
#include "Obj.h"
class CGround :
    public CObj
{
public:
    CGround();
    virtual ~CGround();
public:
    void Initialize() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
private:
    //image
    Gdiplus::Image* m_pImage;
};

