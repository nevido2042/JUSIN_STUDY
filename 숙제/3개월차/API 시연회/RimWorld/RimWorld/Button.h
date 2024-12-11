#pragma once
#include "Obj.h"
class CButton :
    public CObj
{
public:
    CButton();
    virtual ~CButton();

public:
    void Initialize() override;
    int Update() override;
    int Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
protected:
    int		        m_iDrawID;
};

