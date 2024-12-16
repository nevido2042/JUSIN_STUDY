#pragma once
#include "Button.h"

#include "DeconstructBtn.h"

class CCommandBtn :
    public CButton
{
public:
    CCommandBtn();
    virtual ~CCommandBtn();
public:
    void Create_DeconstructBtn();
    void Destroy_DeconstructBtn();

public:
    void Initialize() override;
    void On_Click() override;
    void Release() override;
    //��ü��ư�� ������ ���� Ȯ���ϴ� ��
    bool    m_bActivate;

    CObj*   m_pDeconstructBtn;
};

