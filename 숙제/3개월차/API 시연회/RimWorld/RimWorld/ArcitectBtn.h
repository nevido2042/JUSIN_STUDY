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

    //��ɹ�ư�� ������ ���� Ȯ���ϴ� ��, ������ ��ư��
    bool    m_bActivate;

    //���߿� �ϳ��� Ȱ��ȭ ���Ѿ���
    CObj*   m_pCommandBtn;
    CObj*   m_pStructureBtn;
};

