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

    //¹«±â »ý¼º
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
    CPawn::Render(hDC);

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    /*Ellipse(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
        m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);*/

        // ¸² ¸öÅë, ¾ó±¼, ¸Ó¸® ¼ø
    HDC hTestDC(nullptr);
    switch (m_eDir)
    {
    case UU:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"lancer_north");
        // ¸öÅë
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"lancer_east");
        // ¸öÅë
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"lancer_south");
        // ¸öÅë
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"lancer_west");
        // ¸öÅë
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;
    default:
        break;
    }

    ////Å¸°Ù Ãâ·Â
    //if (m_pTarget)
    //{
    //    // »¡°£»ö Ææ »ý¼º
    //    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // ±½±â 1, »¡°£»ö
    //    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);       // ±âÁ¸ Ææ ÀúÀå ¹× »¡°£»ö Ææ ¼³Á¤

    //    int iX = (int)m_pTarget->Get_Info().fX;
    //    int iY = (int)m_pTarget->Get_Info().fY;

    //    Ellipse(hDC, int(iX + iScrollX - 10), int(iY + iScrollY - 10),
    //        int(iX + 10 + iScrollX), int(iY + 10 + iScrollY));

    //    // Ææ Á¤¸®
    //    SelectObject(hDC, hOldBrush); // ±âÁ¸ Ææ º¹¿ø
    //    DeleteObject(hBrush);         // »¡°£»ö Ææ »èÁ¦
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
