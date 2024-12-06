#pragma once
#include "CObj.h"
class CMonster :   public CObj
{
public:
    CMonster();
    virtual ~CMonster();
    
public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
};

