#pragma once
#include "Obj.h"
class CItem abstract:
    public CObj
{
    // CObj��(��) ���� ��ӵ�
public:
    void Initialize() = 0;
    void Update() override;
    void Relase() override;
    void Render() override;
};

