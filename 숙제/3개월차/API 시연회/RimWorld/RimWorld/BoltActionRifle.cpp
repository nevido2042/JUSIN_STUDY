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
    static_cast<CProjectile*>(pObj)->Set_Owner(m_pTarget);//총알의 주인을 총의 소유자로 설정한다.
}

void CBoltActionRifle::Render(HDC hDC)
{
    //림이 소집상태가 아니면 무기 출력 하지 않도록
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




    // image.png 파일을 이용하여 Image 객체를 생성합니다.
    Gdiplus::Image* image = Gdiplus::Image::FromFile(L"../Image/Things/Weapon/BoltActionRifle.png");
    Graphics Grapics(hDC);

    // 회전의 중심점 설정 (이미지의 중심)
    int centerX = int(m_tRect.left + iScrollX + 64 * 0.5f);
    int centerY = int(m_tRect.top + iScrollY + 64 * 0.5f);

    // 회전 변환 적용
    Grapics.TranslateTransform((REAL)centerX, (REAL)centerY);  // 회전 중심으로 이동
    Grapics.RotateTransform(-m_fAngle);        // 회전 각도 적용
    Grapics.TranslateTransform((REAL)-centerX, (REAL)-centerY); // 원래 위치로 이동

    Grapics.DrawImage(image, m_tRect.left + iScrollX, m_tRect.top + iScrollY, 64, 64);

        

    //무기 출력
    /*HDC hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"BoltActionRifle");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        64, 64,
        hTestDC, 0, 0, 64, 64,
        RGB_WHITE);*/
}
