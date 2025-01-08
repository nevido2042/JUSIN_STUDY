#include "pch.h"
#include "Lancer.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "KeyMgr.h"
#include "ColonyMgr.h"
#include "SteelWall.h"
#include "TileMgr.h"
#include "SoundMgr.h"

CLancer::CLancer()
{
}

CLancer::~CLancer()
{
    Release();
}

void CLancer::Initialize()
{
    CEnemy::Initialize();

    m_fSpeed = 1.f;

    m_fMaxHP = 10.f;
    m_fHP = m_fMaxHP;

    //무기 생성
    m_pRangedWeapon = CAbstractFactory<CChargeLance>::Create(m_tInfo.fX, m_tInfo.fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WEAPON, m_pRangedWeapon);
    m_pRangedWeapon->Set_Target(this);

    //Take_Damage(50.f);

    Change_State(WANDERING);
}

int CLancer::Update()
{
    return CEnemy::Update();
}

void CLancer::Late_Update()
{
    CPawn::Late_Update();
}

void CLancer::Render(HDC hDC)
{

    // 화면 확대/축소 비율을 가져옵니다.
    float fZoom = CCamera::Get_Instance()->Get_Zoom();

    // 월드 좌표를 스크린 좌표로 변환합니다.
    POINT tScreenPos = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left, (float)m_tRect.top);

    // 림 몸통, 얼굴, 머리 순
    HDC hTestDC(nullptr);
    switch (m_eDir)
    {
    case UU:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"lancer_north");
        // 몸통
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x + (- IMAGE_OFFSET_X) * fZoom),  // 위치 보정만 하고 fZoom은 크기 보정에만 사용
            (int)(tScreenPos.y + (- IMAGE_OFFSET_Y) * fZoom),
            (int)(128 * fZoom),  // 크기 보정 (fZoom만 크기 조정)
            (int)(128 * fZoom),
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"lancer_east");
        // 몸통
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x + ( - IMAGE_OFFSET_X) * fZoom),  // 위치 보정만 하고 fZoom은 크기 보정에만 사용
            (int)(tScreenPos.y + ( - IMAGE_OFFSET_Y) * fZoom),
            (int)(128 * fZoom),  // 크기 보정 (fZoom만 크기 조정)
            (int)(128 * fZoom),
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"lancer_south");
        // 몸통
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x + ( - IMAGE_OFFSET_X) * fZoom),  // 위치 보정만 하고 fZoom은 크기 보정에만 사용
            (int)(tScreenPos.y + ( - IMAGE_OFFSET_Y) * fZoom),
            (int)(128 * fZoom),  // 크기 보정 (fZoom만 크기 조정)
            (int)(128 * fZoom),
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"lancer_west");
        // 몸통
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x + ( - IMAGE_OFFSET_X) * fZoom),  // 위치 보정만 하고 fZoom은 크기 보정에만 사용
            (int)(tScreenPos.y + ( - IMAGE_OFFSET_Y) * fZoom),
            (int)(128 * fZoom),  // 크기 보정 (fZoom만 크기 조정)
            (int)(128 * fZoom),
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    default:
        break;
    }



    ////타겟 출력
    //if (m_pTarget)
    //{
    //    // 빨간색 펜 생성
    //    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // 굵기 1, 빨간색
    //    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);       // 기존 펜 저장 및 빨간색 펜 설정

    //    int iX = (int)m_pTarget->Get_Info().fX;
    //    int iY = (int)m_pTarget->Get_Info().fY;

    //    Ellipse(hDC, int(iX + iScrollX - 10), int(iY + iScrollY - 10),
    //        int(iX + 10 + iScrollX), int(iY + 10 + iScrollY));

    //    // 펜 정리
    //    SelectObject(hDC, hOldBrush); // 기존 펜 복원
    //    DeleteObject(hBrush);         // 빨간색 펜 삭제
    //}
    //if (!m_pTarget)
    //{
    //    return;
    //}

}

void CLancer::Dead()
{
    CEnemy::Dead();
    CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
    CSoundMgr::Get_Instance()->PlaySound(L"Pawn_Mech_Scyther_Death_1a.wav", SOUND_EFFECT, .2f);
}
