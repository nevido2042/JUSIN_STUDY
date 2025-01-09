#include "pch.h"
#include "Centipede.h"
#include "BmpMgr.h"

#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "KeyMgr.h"
#include "ColonyMgr.h"
#include "SteelWall.h"
#include "TileMgr.h"
#include "Colony.h"
#include "SceneMgr.h"
#include "TutorialMgr.h"
#include "SoundMgr.h"

CCentipede::CCentipede()
{
}

CCentipede::~CCentipede()
{
	Release();
}

void CCentipede::Initialize()
{
    CEnemy::Initialize();

    m_fSpeed = 1.f;

    m_fMaxHP = 50.f;
    m_fHP = m_fMaxHP;

    //무기 생성
    m_pRangedWeapon = CAbstractFactory<CChargeBlasterLight>::Create(m_tInfo.fX, m_tInfo.fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WEAPON, m_pRangedWeapon);
    m_pRangedWeapon->Set_Target(this);

    //Take_Damage(50.f);

    Change_State(WANDERING);
}

int CCentipede::Update()
{
    if (m_bDead)
    {
        if (!CColonyMgr::Get_Instance()->Get_ShipBtnActive())
        {
            CColonyMgr::Get_Instance()->Set_ShipBtnActive();
            CColonyMgr::Get_Instance()->Create_ShipBtn();

            if (CTutorialMgr::Get_Instance()->Get_CurQuest() == CTutorialMgr::QUEST_BATTLE)
            {
                CTutorialMgr::Get_Instance()->IncreaseQuestProgress();
            }
        }
    }

    return CEnemy::Update();
}

void CCentipede::Late_Update()
{
    CPawn::Late_Update();
}

void CCentipede::Render(HDC hDC)
{
    CPawn::Render(hDC);

    // 월드 좌표를 스크린 좌표로 변환합니다.
    POINT tScreenPos = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left, (float)m_tRect.top);

    // 확대/축소 비율을 가져옵니다.
    float fZoom = CCamera::Get_Instance()->Get_Zoom();

    // 림 몸통, 얼굴, 머리 순
    HDC hTestDC(nullptr);
    switch (m_eDir)
    {
    case UU:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_north");
        // 몸통
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x + ( - IMAGE_OFFSET_X) * fZoom), // 좌표 보정 후 fZoom을 적용
            (int)(tScreenPos.y + ( - IMAGE_OFFSET_Y) * fZoom),
            (int)(128 * fZoom),  // 크기 보정 (fZoom만 크기 조정)
            (int)(128 * fZoom),
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_east");
        // 몸통
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x + ( - IMAGE_OFFSET_X) * fZoom), // 좌표 보정 후 fZoom을 적용
            (int)(tScreenPos.y + ( - IMAGE_OFFSET_Y) * fZoom),
            (int)(128 * fZoom),  // 크기 보정 (fZoom만 크기 조정)
            (int)(128 * fZoom),
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_south");
        // 몸통
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x + ( - IMAGE_OFFSET_X) * fZoom), // 좌표 보정 후 fZoom을 적용
            (int)(tScreenPos.y + ( - IMAGE_OFFSET_Y) * fZoom),
            (int)(128 * fZoom),  // 크기 보정 (fZoom만 크기 조정)
            (int)(128 * fZoom),
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_west");
        // 몸통
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x + ( - IMAGE_OFFSET_X) * fZoom), // 좌표 보정 후 fZoom을 적용
            (int)(tScreenPos.y + ( - IMAGE_OFFSET_Y) * fZoom),
            (int)(128 * fZoom),  // 크기 보정 (fZoom만 크기 조정)
            (int)(128 * fZoom),
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    default:
        break;
    }

}

void CCentipede::Dead()
{
    CEnemy::Dead();
    CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
    CSoundMgr::Get_Instance()->PlaySound(L"Pawn_Mechanoid_Centipede_Death_1a.wav", SOUND_EFFECT, .2f);
}
