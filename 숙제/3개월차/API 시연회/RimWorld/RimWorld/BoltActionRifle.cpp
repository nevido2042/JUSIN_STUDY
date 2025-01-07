#include "pch.h"
#include "BoltActionRifle.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "Rim.h"
#include "SoundMgr.h"
#include <gdiplus.h>

CBoltActionRifle::CBoltActionRifle()
{
}

CBoltActionRifle::~CBoltActionRifle()
{
	Release();
}

void CBoltActionRifle::Initialize()
{
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_WEAPON;

    //�� �߻� ����
    m_fFireRate = 100.f;
    m_fReloadSpeed = 1.f;
    //���� �����Ÿ�
    m_fRange = 500.f;

    m_pImage = Gdiplus::Image::FromFile(L"../Image/Things/Weapon/BoltActionRifle.png");
}

void CBoltActionRifle::Fire()
{
    if (!Try_Fire())
    {
        return;
    }

    CSoundMgr::Get_Instance()->StopSound(SOUND_BOLTACTION);//�� �Ҹ� �������� ���� ���� �ѼҸ� ��������
    CSoundMgr::Get_Instance()->PlaySound(L"GunShotA.wav", SOUND_BOLTACTION, 0.5f);

    CObj* pObj = CAbstractFactory<CBullet_Small>::Create(m_tInfo.fX, m_tInfo.fY, m_fAngle);
    CObjMgr::Get_Instance()->Add_Object(OBJ_PROJECTILE, pObj);
    static_cast<CProjectile*>(pObj)->Set_Owner(m_pTarget);//�Ѿ��� ������ ���� �����ڷ� �����Ѵ�.
}

void CBoltActionRifle::Render(HDC hDC)
{
    //���� �������°� �ƴϸ� ���� ��� ���� �ʵ���
    if (m_pTarget)
    {
        if (CRim* pTargetRim = dynamic_cast<CRim*>(m_pTarget))
        {
            if (pTargetRim->Get_State() != CRim::DRAFTED)
            {
                return;
            }
        }
    }

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    POINT tPoint = CCamera::Get_Instance()->WorldToScreen(m_tRect.left, m_tRect.top);
    float fZoom = CCamera::Get_Instance()->Get_Zoom();
    Graphics Grapics(hDC);

    // ȸ���� �߽��� ���� (�̹����� �߽�)
    int centerX = int(tPoint.x + iScrollX + 64 * 0.5f);
    int centerY = int(tPoint.y + iScrollY + 64 * 0.5f);

    // ȸ�� ��ȯ ����
    Grapics.TranslateTransform((REAL)centerX, (REAL)centerY);  // ȸ�� �߽����� �̵�
    Grapics.RotateTransform(-m_fAngle);                       // ȸ�� ���� ����

    // ���� ���� ����
    if (270.f > m_fAngle && 90.f < m_fAngle) // ������ �ٶ󺸴� ��� �÷���
    {
        Grapics.ScaleTransform(1.0f, -1.0f); // Y���� ����
    }

    // ���� ��ġ�� �̵�
    Grapics.TranslateTransform((REAL)-centerX, (REAL)-centerY);

    // �̹��� �׸���
    Grapics.DrawImage(m_pImage, tPoint.x + iScrollX, tPoint.y + iScrollY, 64 * fZoom, 64 * fZoom);

}
