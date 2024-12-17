#pragma once
#include "Obj.h"
class CButton abstract:
    public CObj
{
public:
    CButton();
    virtual ~CButton();
public:
    void    Set_Parent(CObj* _pObj) { m_pParent = _pObj; }
public:
    virtual void On_Click();
    virtual void Set_Activate(bool _bActivate);
public:
    void Initialize() PURE;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC);
    void Release() override;
    
protected:
    CObj* m_pParent;
};

