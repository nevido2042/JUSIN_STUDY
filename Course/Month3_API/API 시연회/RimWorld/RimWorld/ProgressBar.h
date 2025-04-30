#pragma once
#include "Obj.h"
class CProgressBar :
    public CObj
{
public:
    CProgressBar();
    virtual ~CProgressBar();
public:
    void Set_Value(float _fCurValue, float _fMaxValue);
public:
    void Initialize() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
private:
    float m_fCurValue;
    float m_fMaxValue;
};

