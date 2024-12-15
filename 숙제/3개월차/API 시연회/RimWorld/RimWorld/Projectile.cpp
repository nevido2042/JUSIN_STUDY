#include "pch.h"
#include "Projectile.h"
#include "TimeMgr.h"
#include "Pawn.h"

CProjectile::CProjectile()
    :m_fDamage(0.f), m_pOwner(nullptr)
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

void CProjectile::OnCollision(OBJID _eID, CObj* _pOther)
{
    //총알을 쏜 주인이 부딫히면 무시한다.
    if (m_pOwner == _pOther)
    {
        return;
    }
    //림이거나, 메카노이드면 데미지주고 없어진다.
    if (_eID == OBJ_RIM || _eID == OBJ_MECHANOID)
    {
        static_cast<CPawn*>(_pOther)->Take_Damage(m_fDamage);
        Set_Destroyed();
    }
}
