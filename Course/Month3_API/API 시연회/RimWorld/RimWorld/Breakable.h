#pragma once
#include "Obj.h"
class CBreakable :
    public CObj
{
public:
    CBreakable();
    virtual ~CBreakable();
public:
    void    Take_Damage(float _fDamage);
public:
    void    Set_TaskReserved(bool _b) { m_bTaskReserved = _b; }

    bool    Get_IsBrokenDown() { return m_bBrokendown; }
    void    Set_IsBrokenDown() { m_bBrokendown = true; }
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
protected:
    //³»±¸µµ(HP´À³¦)
    float   m_fDurability;
    float   m_fMaxDurability;
    bool    m_bBrokendown;

    bool    m_bTaskReserved;
};

