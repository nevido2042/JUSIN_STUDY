#pragma once
#include "Pawn.h"
class CMonster :
    public CPawn
{
public:
    CMonster();
    virtual ~CMonster();
public:
    void Initialize() override;
    void Update() override;
    void Release() override;
};

