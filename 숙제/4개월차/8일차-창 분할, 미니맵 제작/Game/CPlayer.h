#pragma once
#include "CObj.h"
class CPlayer :
    public CObj
{
    // CObj��(��) ���� ��ӵ�
    void Initialize() override;
    void Update() override;
    void Render() override;
    void Release() override;
};

