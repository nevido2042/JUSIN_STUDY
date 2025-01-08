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

    //���� ����
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

    // ȭ�� Ȯ��/��� ������ �����ɴϴ�.
    float fZoom = CCamera::Get_Instance()->Get_Zoom();

    // ���� ��ǥ�� ��ũ�� ��ǥ�� ��ȯ�մϴ�.
    POINT tScreenPos = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left, (float)m_tRect.top);

    // �� ����, ��, �Ӹ� ��
    HDC hTestDC(nullptr);
    switch (m_eDir)
    {
    case UU:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"lancer_north");
        // ����
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x + (- IMAGE_OFFSET_X) * fZoom),  // ��ġ ������ �ϰ� fZoom�� ũ�� �������� ���
            (int)(tScreenPos.y + (- IMAGE_OFFSET_Y) * fZoom),
            (int)(128 * fZoom),  // ũ�� ���� (fZoom�� ũ�� ����)
            (int)(128 * fZoom),
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"lancer_east");
        // ����
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x + ( - IMAGE_OFFSET_X) * fZoom),  // ��ġ ������ �ϰ� fZoom�� ũ�� �������� ���
            (int)(tScreenPos.y + ( - IMAGE_OFFSET_Y) * fZoom),
            (int)(128 * fZoom),  // ũ�� ���� (fZoom�� ũ�� ����)
            (int)(128 * fZoom),
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"lancer_south");
        // ����
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x + ( - IMAGE_OFFSET_X) * fZoom),  // ��ġ ������ �ϰ� fZoom�� ũ�� �������� ���
            (int)(tScreenPos.y + ( - IMAGE_OFFSET_Y) * fZoom),
            (int)(128 * fZoom),  // ũ�� ���� (fZoom�� ũ�� ����)
            (int)(128 * fZoom),
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"lancer_west");
        // ����
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x + ( - IMAGE_OFFSET_X) * fZoom),  // ��ġ ������ �ϰ� fZoom�� ũ�� �������� ���
            (int)(tScreenPos.y + ( - IMAGE_OFFSET_Y) * fZoom),
            (int)(128 * fZoom),  // ũ�� ���� (fZoom�� ũ�� ����)
            (int)(128 * fZoom),
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    default:
        break;
    }



    ////Ÿ�� ���
    //if (m_pTarget)
    //{
    //    // ������ �� ����
    //    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // ���� 1, ������
    //    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);       // ���� �� ���� �� ������ �� ����

    //    int iX = (int)m_pTarget->Get_Info().fX;
    //    int iY = (int)m_pTarget->Get_Info().fY;

    //    Ellipse(hDC, int(iX + iScrollX - 10), int(iY + iScrollY - 10),
    //        int(iX + 10 + iScrollX), int(iY + 10 + iScrollY));

    //    // �� ����
    //    SelectObject(hDC, hOldBrush); // ���� �� ����
    //    DeleteObject(hBrush);         // ������ �� ����
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
