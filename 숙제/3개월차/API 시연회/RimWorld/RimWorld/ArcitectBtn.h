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

    //��ɹ�ư�� ������ ���� Ȯ���ϴ� ��
    bool    m_bActivate;

    CObj*   m_pCommandBtn;
};

