#include "pch.h"
#include "Projectile.h"

CProjectile::CProjectile()
    :m_fDamage(0.f)
{
}

CProjectile::~CProjectile()
{
    Release();
}

void CProjectile::Initialize()
{
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_GAMEOBJECT;

    m_fSpeed = 10.f;

    m_fDamage = 3.f;
}

int CProjectile::Update()
{
    if (m_bDestroyed)
        return OBJ_DESTROYED;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CProjectile::Late_Update()
{
    //Angle방향대로 날라간다.
    float fRadian = m_fAngle * (PI / 180.f);

    if (m_fAngle > 180.f)
    {
        fRadian = (2.f * PI) - fRadian;
    }

    //m_fAngle = fRadian * (180.f / PI);

    m_tInfo.fX += m_fSpeed * cosf(fRadian);
    m_tInfo.fY -= m_fSpeed * sinf(fRadian);
}

void CProjectile::Release()
{
}
