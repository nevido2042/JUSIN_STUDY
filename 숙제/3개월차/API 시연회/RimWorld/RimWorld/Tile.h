#pragma once
#include "Obj.h"
//Ÿ���� Ÿ�ϸŴ����� ���ؼ��� ��������
class CTile :
    public CObj
{
public:
    CTile();
    virtual ~CTile();
public:
    void Initialize() override;
    int Update() override;
    int Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

