#pragma once
#include "Obj.h"
class CEffect :
    public CObj
{
public:
    CEffect();
    virtual ~CEffect();
public:
    void Set_LifeTime(float _fLifeTime) { m_fLifeTime = _fLifeTime; }
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
private:
    float   m_fLifeTime;
    float   m_fTime;
};

