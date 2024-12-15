#include "pch.h"
#include "Projectile.h"
#include "TimeMgr.h"

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

    m_fDamage = 10.f;
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

    m_tInfo.fX += m_fSpeed * cosf(fRadian) * GAMESPEED;
    m_tInfo.fY -= m_fSpeed * sinf(fRadian) * GAMESPEED;
}

void CProjectile::Release()
{
}
