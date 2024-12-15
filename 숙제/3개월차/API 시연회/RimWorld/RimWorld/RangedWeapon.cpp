#include "pch.h"
#include "RangedWeapon.h"
#include "TimeMgr.h"
#include "Pawn.h"

CRangedWeapon::CRangedWeapon()
    :m_fFireRate(0), m_pProjectile(nullptr),
    m_fRange(0.f),m_fElapsedTimeSinceShot(0.f), m_fReloadSpeed(0.f)
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

bool CRangedWeapon::Try_Fire()
{
    //발사를 시도했는데

        /*  발사 후 지난 시간 += 게임 스피드 * 장전 속도

        발사 후 지난 시간이 > 다음 발사 시간

        발사

        발사 후
        발사후 지난시간 0, 다음 발사시간 SET*/

    //다음 발사시간보다 이르면 발사 실패
    if (m_fElapsedTimeSinceShot < m_fFireRate)
    {
        return false;
    }

    m_fElapsedTimeSinceShot = 0.f;

    //타겟(무기 주인)이 없으면 리턴
    if (!m_pTarget)
    {
        return false;
    }
    CObj* pTarget = m_pTarget->Get_Target();
    //무기 주인의 타겟이 없으면 리턴
    if (!pTarget)
    {
        return false;
    }

    m_fAngle = static_cast<CPawn*>(m_pTarget)->Get_TargetAngle();

    return true;
}

void CRangedWeapon::Initialize()
{
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_WEAPON;

    //총 발사 관련
    m_fFireRate = 100.f;
    m_fReloadSpeed = 1.f;
    //무기 사정거리
    m_fRange = 500.f;
}

int CRangedWeapon::Update()
{
    if (m_bDestroyed)
        return OBJ_DESTROYED;

    //발사 후 지난 시간 += 게임 스피드 * 장전 속도
    m_fElapsedTimeSinceShot += GAMESPEED * m_fReloadSpeed;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CRangedWeapon::Late_Update()
{
    Follow_Pawn();    
}

void CRangedWeapon::Release()
{
}
