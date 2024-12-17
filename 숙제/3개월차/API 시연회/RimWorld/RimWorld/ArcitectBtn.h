#pragma once
#include "Button.h"

#include "CommandBtn.h"
#include "StructureBtn.h"

class CArcitectBtn :
    public CButton
{
public:
    CArcitectBtn();
    virtual ~CArcitectBtn();
public:
    void Create_CommandBtn();
    void Destroy_CommandBtn();
    void Create_StructureBtn();
    void Destroy_StructureBtn();
    void On_Click() override;
public:
    void Initialize() override;

    //명령버튼을 만들지 말지 확인하는 것, 구조물 버튼도
    bool    m_bActivate;

    //둘중에 하나만 활성화 시켜야함
    CObj*   m_pCommandBtn;
    CObj*   m_pStructureBtn;
};

