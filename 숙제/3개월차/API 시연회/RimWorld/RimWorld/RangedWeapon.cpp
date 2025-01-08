#include "pch.h"
#include "RangedWeapon.h"
#include "TimeMgr.h"
#include "Pawn.h"
#include "ScrollMgr.h"

CRangedWeapon::CRangedWeapon()
    :m_fFireRate(0), m_pProjectile(nullptr), m_pImage(nullptr),
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
    const CObj* pTarget = m_pTarget->Get_Target();
    //무기 주인의 타겟이 없으면 리턴
    if (!pTarget)
    {
        return false;
    }

    /*m_fAngle = static_cast<CPawn*>(m_pTarget)->Get_TargetAngle();*/

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
    m_fElapsedTimeSinceShot += GAMESPEED * m_fReloadSpeed; //프레임당 시간지나는거 굳이 안넣어도 되긴하네

    //적 계속 바라보도록
    m_fAngle = static_cast<CPawn*>(m_pTarget)->Get_TargetAngle();

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CRangedWeapon::Late_Update()
{
    if (CPawn* pPawn = dynamic_cast<CPawn*>(m_pTarget))
    {
        if (pPawn->Get_IsDead())
        {
            Set_Destroyed();
        }
    }

    Follow_Pawn();    
}

void CRangedWeapon::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    POINT tPoint = CCamera::Get_Instance()->WorldToScreen(m_tRect.left, m_tRect.top);
    float fZoom = CCamera::Get_Instance()->Get_Zoom();


    // image.png 파일을 이용하여 Image 객체를 생성합니다.

    Graphics Grapics(hDC);

    POINT tCenterPoint = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left + 64 * 0.5f, (float)m_tRect.top + 64 * 0.5f);
    // 회전의 중심점 설정 (이미지의 중심)
    int centerX = tCenterPoint.x;// int(tPoint.x + iScrollX + 64 * 0.5f);
    int centerY = tCenterPoint.y;// int(tPoint.y + iScrollY + 64 * 0.5f);

    // 회전 변환 적용
    Grapics.TranslateTransform((REAL)centerX, (REAL)centerY);  // 회전 중심으로 이동
    Grapics.RotateTransform(-m_fAngle);        // 회전 각도 적용
    Grapics.TranslateTransform((REAL)-centerX, (REAL)-centerY); // 원래 위치로 이동

    Grapics.DrawImage(m_pImage, tPoint.x + iScrollX, tPoint.y + iScrollY, int(64 * fZoom), int(64 * fZoom));


    //무기 출력
    /*HDC hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"ChargeBlasterLight");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        64, 64,
        hTestDC, 0, 0, 64, 64,
        RGB_WHITE);*/
}

void CRangedWeapon::Release()
{
}
