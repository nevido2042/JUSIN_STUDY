#pragma once
#include "CObj.h"
class CTile :
    public CObj
{
public:
    CTile();
    virtual ~CTile();
public:
    void Initialize() override;
    void Render(CDevice* _pDevice) override;
};

