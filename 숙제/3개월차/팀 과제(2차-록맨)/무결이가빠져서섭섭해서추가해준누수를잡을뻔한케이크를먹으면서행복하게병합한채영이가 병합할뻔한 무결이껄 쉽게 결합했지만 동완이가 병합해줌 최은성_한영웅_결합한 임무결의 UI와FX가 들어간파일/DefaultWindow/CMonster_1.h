#pragma once
#include "CMonster.h"

class CMonster_1 : public CMonster
{
public:
    CMonster_1();
	~CMonster_1();

public:

    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;


private:
    HDC		hMemDC; // ��Ʈ�� ������������ �̸�����
    float fGravity;

};

