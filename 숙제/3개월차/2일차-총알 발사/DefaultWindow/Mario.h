#pragma once
#include "CObj.h"
class CMario :
    public CObj
{
public:
    CMario();
public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Render(HDC hDC) override;
    void Release() override;
private:
    void Key_Input();
private:
    bool m_bJump;
    ULONGLONG m_ullJumpTime;
    bool m_bGround;
    float		m_fFallSpeed;
    float       m_fGravity;
};

