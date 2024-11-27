#include "pch.h"
#include "CMonster.h"

CMonster::CMonster()
{
}

CMonster::~CMonster()
{
    Release();
}

void CMonster::Initialize()
{
    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 50.f;
    m_fSpeed = 3.f;
}

int CMonster::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    m_tInfo.fX += m_fSpeed;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CMonster::Late_Update()
{
    if (100 >= m_tRect.left || WINCX - 100 <= m_tRect.right)
        m_fSpeed *= -1.f;
}

void CMonster::Render(HDC hDC)
{
    Ellipse(hDC,
        m_tRect.left,
        m_tRect.top,
        m_tRect.right,
        m_tRect.bottom);
}

void CMonster::Release()
{
}
