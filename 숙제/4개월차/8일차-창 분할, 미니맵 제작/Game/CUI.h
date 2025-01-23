#pragma once
#include "CObj.h"
class CUI :
    public CObj
{
public:
    CUI();
    virtual~CUI();
public:
    void Initialize() override;
    void Late_Update() override;
    void Render() override;
    void Release() override;
};

