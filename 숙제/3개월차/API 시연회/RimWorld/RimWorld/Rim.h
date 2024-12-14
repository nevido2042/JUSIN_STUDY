#pragma once
#include "Pawn.h"
#include "Node.h"

class CRim :
    public CPawn
{
public:
    CRim();
    virtual ~CRim();

public:
    void Initialize() override;
    void Render(HDC hDC) override;

protected:
    void Dead() override;
};
