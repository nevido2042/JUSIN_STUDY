#pragma once
#include "Button.h"
class CArcitectBtn :
    public CButton
{
public:
    CArcitectBtn();
    virtual ~CArcitectBtn();
public:
    void Initialize() override;
};

