#pragma once
#include "CObj.h"
class CBlock :
    public CObj
{
public:
    CBlock();
    CBlock(float _fX, float _fY, float _fSize);
    virtual ~CBlock();
public:
    INFO*			Get_Info() { return &m_tInfo; }

public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

protected:
    BLOCKTYPE m_eBlockType;
};

