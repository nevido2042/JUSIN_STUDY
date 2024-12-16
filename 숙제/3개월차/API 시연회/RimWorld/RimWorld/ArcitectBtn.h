#pragma once
#include "Button.h"

#include "CommandBtn.h"

class CArcitectBtn :
    public CButton
{
public:
    CArcitectBtn();
    virtual ~CArcitectBtn();
public:
    void Create_CommandBtn();
    void Destroy_CommandBtn();
    void On_Click() override;
public:
    void Initialize() override;

    //명령버튼을 만들지 말지 확인하는 것
    bool    m_bActivate;

    CObj*   m_pCommandBtn;
};

