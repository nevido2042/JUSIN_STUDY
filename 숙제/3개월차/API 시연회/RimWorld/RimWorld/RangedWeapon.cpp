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
    //무기주인의 타겟과 자신의 위치의 각도를 계산
    float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f);

    //타겟(무기 주인이 없으면 리턴)
    if (!m_pTarget)
    {
        return;
    }
    CObj* pTarget = m_pTarget->Get_Target();
    //무기 주인의, 타겟이 없으면 리턴
    if (!pTarget)
    {
        return;
    }

    fWidth = pTarget->Get_Info().fX - m_tInfo.fX;
    fHeight = pTarget->Get_Info().fY - m_tInfo.fY;

    fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

    fRadian = acosf(fWidth / fDiagonal);

    if (pTarget->Get_Info().fY > m_tInfo.fY)
        fRadian = (2.f * PI) - fRadian;

    m_fAngle = fRadian * (180.f / PI);
}

void CRangedWeapon::Initialize()
{
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_WEAPON;

    //총 발사 관련
    m_ullFireRate = 800.f;
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
        m_ullLastFireTime = GetTickCount64();
    }
    
}

void CRangedWeapon::Release()
{
}
