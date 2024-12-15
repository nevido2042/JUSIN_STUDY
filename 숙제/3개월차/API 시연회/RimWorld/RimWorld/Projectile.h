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
    void  Set_Owner(CObj* _pObj) { m_pOwner = _pObj; }
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) PURE;
    void Release() override;
    void OnCollision(OBJID _eID, CObj* _pOther) override;
protected:
    float m_fDamage;
    CObj* m_pOwner;
};

