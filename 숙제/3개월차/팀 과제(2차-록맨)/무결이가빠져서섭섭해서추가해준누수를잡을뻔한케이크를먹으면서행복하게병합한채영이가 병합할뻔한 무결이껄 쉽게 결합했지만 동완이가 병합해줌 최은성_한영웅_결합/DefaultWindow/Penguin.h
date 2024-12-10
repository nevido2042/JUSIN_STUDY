#pragma once
#include "CMonster.h"
class CPenguin :
    public CMonster
{
public:
    CPenguin();
    virtual ~CPenguin();
public:
    virtual void Initialize() override;
    virtual void Render(HDC hDC) override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Release() override;
private:
    IMAGE m_tFly1;
    IMAGE m_tFly2;

    IMAGE m_tCurImage;
};

