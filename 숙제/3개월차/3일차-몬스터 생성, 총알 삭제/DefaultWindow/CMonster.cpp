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
    m_tInfo.fCX = 25.f;
    m_tInfo.fCY = 25.f;
    m_fSpeed = 3.f;
}

int CMonster::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    m_tInfo.fY += m_fSpeed;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CMonster::Late_Update()
{
    /*if (100 >= m_tRect.left || WINCX - 100 <= m_tRect.right)
        m_fSpeed *= -1.f;*/

    if (GAME_WIN_BOTTOM - m_tInfo.fCY * 0.5f < m_tInfo.fY)
    {
        m_tInfo.fY = GAME_WIN_TOP;
    }
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
