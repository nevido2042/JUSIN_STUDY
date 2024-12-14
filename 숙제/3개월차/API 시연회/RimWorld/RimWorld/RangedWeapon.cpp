#include "pch.h"
#include "RangedWeapon.h"

CRangedWeapon::CRangedWeapon()
    :m_ullFireRate(0), m_pProjectile(nullptr), m_ullLastFireTime(0)
{
}

CRangedWeapon::~CRangedWeapon()
{
    Release();
}


void CRangedWeapon::Follow_Pawn()
{
    Set_Pos(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
}

void CRangedWeapon::Fire()
{

}

void CRangedWeapon::Initialize()
{
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_WEAPON;

    //รั น฿ป็ ฐüทร
    m_ullFireRate = 300.f;
    m_ullLastFireTime = GetTickCount64();
}

int CRangedWeapon::Update()
{
    if (m_bDestroyed)
        return OBJ_DESTROYED;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CRangedWeapon::Late_Update()
{
    Follow_Pawn();

    if (m_ullLastFireTime + m_ullFireRate < GetTickCount64())
    {
        Fire();
    }
    
}

void CRangedWeapon::Release()
{
}
