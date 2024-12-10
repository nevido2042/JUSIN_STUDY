#pragma once
#include "CMonster.h"
class CFireWall : public CMonster
{
public:
    CFireWall() : m_bIdle(false), m_fOrigY(0.f), m_iRenderFrame(120), m_bMoveUp(true), m_bFrozen(false) {}
    ~CFireWall() { Release(); }
public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
private:
    int         m_iRenderFrame;
    ULONGLONG   m_ullTime;
    float       m_fOrigY;
    bool        m_bMoveUp;
    bool        m_bIdle;
    bool        m_bFrozen;
};

