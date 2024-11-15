#pragma once
#include "Obj.h"
class CPlayer :
    public CObj
{
public:
    CPlayer();
    virtual ~CPlayer();
public:
    virtual void Render() override;
    
};

