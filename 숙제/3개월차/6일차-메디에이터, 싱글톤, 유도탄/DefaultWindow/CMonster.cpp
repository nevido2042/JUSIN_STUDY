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

    float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f);

    fWidth = m_pTarget->Get_Info().fX - m_tInfo.fX;
    fHeight = m_pTarget->Get_Info().fY - m_tInfo.fY;

    fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

    fRadian = acosf(fWidth / fDiagonal);

  //   if (m_pTarget->Get_Info().fY > m_tInfo.fY)
  //       fRadian = (2.f * PI) - fRadian;

    // radian to degree
    //m_fAngle = fRadian * (180.f / PI);
    //
    //if (m_pTarget->Get_Info().fY > m_tInfo.fY)
    //    m_fAngle *= -1.f;
    //
    //// degree to radian
    //m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
    //m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CMonster::Late_Update()
{
    //if (100 >= m_tRect.left || WINCX - 100 <= m_tRect.right)
    //    m_fSpeed *= -1.f;
}

void CMonster::Render(HDC hDC)
{
    Rectangle(hDC,
        m_tRect.left,
        m_tRect.top,
        m_tRect.right,
        m_tRect.bottom);
}

void CMonster::Release()
{
}

void CMonster::Start()
{
}
