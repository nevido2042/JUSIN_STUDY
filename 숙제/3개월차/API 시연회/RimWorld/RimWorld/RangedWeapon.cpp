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
    //�߻縦 �õ��ߴµ�

        /*  �߻� �� ���� �ð� += ���� ���ǵ� * ���� �ӵ�

        �߻� �� ���� �ð��� > ���� �߻� �ð�

        �߻�

        �߻� ��
        �߻��� �����ð� 0, ���� �߻�ð� SET*/

    //���� �߻�ð����� �̸��� �߻� ����
    if (m_fElapsedTimeSinceShot < m_fFireRate)
    {
        return false;
    }

    m_fElapsedTimeSinceShot = 0.f;

    //Ÿ��(���� ����)�� ������ ����
    if (!m_pTarget)
    {
        return false;
    }
    const CObj* pTarget = m_pTarget->Get_Target();
    //���� ������ Ÿ���� ������ ����
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

    //�� �߻� ����
    m_fFireRate = 100.f;
    m_fReloadSpeed = 1.f;
    //���� �����Ÿ�
    m_fRange = 500.f;
}

int CRangedWeapon::Update()
{
    if (m_bDestroyed)
        return OBJ_DESTROYED;

    //�߻� �� ���� �ð� += ���� ���ǵ� * ���� �ӵ�
    m_fElapsedTimeSinceShot += GAMESPEED * m_fReloadSpeed; //�����Ӵ� �ð������°� ���� �ȳ־ �Ǳ��ϳ�

    //�� ��� �ٶ󺸵���
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


    // image.png ������ �̿��Ͽ� Image ��ü�� �����մϴ�.

    Graphics Grapics(hDC);

    POINT tCenterPoint = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left + 64 * 0.5f, (float)m_tRect.top + 64 * 0.5f);
    // ȸ���� �߽��� ���� (�̹����� �߽�)
    int centerX = tCenterPoint.x;// int(tPoint.x + iScrollX + 64 * 0.5f);
    int centerY = tCenterPoint.y;// int(tPoint.y + iScrollY + 64 * 0.5f);

    // ȸ�� ��ȯ ����
    Grapics.TranslateTransform((REAL)centerX, (REAL)centerY);  // ȸ�� �߽����� �̵�
    Grapics.RotateTransform(-m_fAngle);        // ȸ�� ���� ����
    Grapics.TranslateTransform((REAL)-centerX, (REAL)-centerY); // ���� ��ġ�� �̵�

    Grapics.DrawImage(m_pImage, tPoint.x + iScrollX, tPoint.y + iScrollY, int(64 * fZoom), int(64 * fZoom));


    //���� ���
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
