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

void CBoltActionRifle::Fire()
{
    if (!Try_Fire())
    {
        return;
    }

    CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
    CSoundMgr::Get_Instance()->PlaySound(L"GunShotA.wav", SOUND_EFFECT, 0.3f);

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




    // image.png ������ �̿��Ͽ� Image ��ü�� �����մϴ�.
    Gdiplus::Image* image = Gdiplus::Image::FromFile(L"../Image/Things/Weapon/BoltActionRifle.png");
    Graphics Grapics(hDC);

    // ȸ���� �߽��� ���� (�̹����� �߽�)
    int centerX = int(m_tRect.left + iScrollX + 64 * 0.5f);
    int centerY = int(m_tRect.top + iScrollY + 64 * 0.5f);

    // ȸ�� ��ȯ ����
    Grapics.TranslateTransform((REAL)centerX, (REAL)centerY);  // ȸ�� �߽����� �̵�
    Grapics.RotateTransform(-m_fAngle);        // ȸ�� ���� ����
    Grapics.TranslateTransform((REAL)-centerX, (REAL)-centerY); // ���� ��ġ�� �̵�

    Grapics.DrawImage(image, m_tRect.left + iScrollX, m_tRect.top + iScrollY, 64, 64);

        

    //���� ���
    /*HDC hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"BoltActionRifle");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        64, 64,
        hTestDC, 0, 0, 64, 64,
        RGB_WHITE);*/
}
