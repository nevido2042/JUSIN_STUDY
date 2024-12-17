#pragma once
#include "Button.h"

#include "SteelWallBtn.h"

class CStructureBtn :
    public CButton
{
public:
    CStructureBtn();
    virtual ~CStructureBtn();
public:
    void Create_SteelWallBtn();
    void Destroy_SteelWallBtn();
public:
    void Initialize() override;
    void On_Click() override;
    void Release() override;
    void Set_Activate(bool _bActivate) override;
    //��ü��ư�� ������ ���� Ȯ���ϴ� ��
    bool    m_bActivate;

    CObj*   m_pSteelBtn;
};

