#include "pch.h"
#include "RangedWeapon.h"
#include "TimeMgr.h"

CRangedWeapon::CRangedWeapon()
    :m_iFireRate(0), m_pProjectile(nullptr),
    m_iLastFireTime(0), m_fRange(0.f)
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
    //너무 발사속도보다 빨리 쐈다면
    int iCurFrame = CTimeMgr::Get_Instance()->Get_CurrentFrame();
    if (m_iLastFireTime + m_iFireRate > iCurFrame)
    {
        //발사 취소
        return false;
    }

    //발사 했음. 발사 시간 저장
    m_iLastFireTime = CTimeMgr::Get_Instance()->Get_CurrentFrame();

    //무기주인의 타겟과 자신의 위치의 각도를 계산
    float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f);

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

    fWidth = pTarget->Get_Info().fX - m_tInfo.fX;
    fHeight = pTarget->Get_Info().fY - m_tInfo.fY;

    fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

    fRadian = acosf(fWidth / fDiagonal);

    if (pTarget->Get_Info().fY > m_tInfo.fY)
        fRadian = (2.f * PI) - fRadian;

    m_fAngle = fRadian * (180.f / PI);

    return true;
}

void CRangedWeapon::Initialize()
{
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_WEAPON;

    //총 발사 관련
    m_iFireRate = 100;
    m_iLastFireTime = CTimeMgr::Get_Instance()->Get_CurrentFrame();
    //무기 사정거리
    m_fRange = 500.f;
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
}

void CRangedWeapon::Release()
{
}
