#include "pch.h"
#include "Projectile.h"

CProjectile::CProjectile()
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
}

void CProjectile::Release()
{
}
