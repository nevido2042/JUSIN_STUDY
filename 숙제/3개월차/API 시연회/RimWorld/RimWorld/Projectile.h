#pragma once
#include "Obj.h"
class CProjectile abstract:
    public CObj
{
public:
    CProjectile();
    virtual ~CProjectile();
public:
    float Get_Damage() { return m_fDamage; }

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) PURE;
    void Release() override;
protected:
    float m_fDamage;
};

