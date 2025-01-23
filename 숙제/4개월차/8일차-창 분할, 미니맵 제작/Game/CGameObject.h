#pragma once
#include "CObj.h"
class CGameObject :
    public CObj
{
public:
    CGameObject();
    virtual ~CGameObject();
public:
    void Initialize() override;
    void Late_Update() override;
    void Render() override;
    void Release() override;
};

