#include "pch.h"
#include "ChargeBlasterLight.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

CChargeBlasterLight::CChargeBlasterLight()
{
}

CChargeBlasterLight::~CChargeBlasterLight()
{
    Release();
}

void CChargeBlasterLight::Initialize()
{
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_WEAPON;

    //총 발사 관련
    m_fFireRate = 10.f;
    m_fReloadSpeed = 1.f;
    //무기 사정거리
    m_fRange = 500.f;

    m_pImage = Gdiplus::Image::FromFile(L"../Image/Things/Weapon/ChargeBlasterLight.png");
}

void CChargeBlasterLight::Fire()
{
    if (!Try_Fire())
    {
        return;
    }

    CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
    CSoundMgr::Get_Instance()->PlaySound(L"ChargeShotA.wav", SOUND_EFFECT, 0.2f);

    //각도 랜덤
    int iRand = (rand() % 21) - 10; //-10 ~ 10

    CObj* pObj = CAbstractFactory<CCharge_Small>::Create(m_tInfo.fX, m_tInfo.fY, float(m_fAngle + iRand));
    CObjMgr::Get_Instance()->Add_Object(OBJ_PROJECTILE, pObj);
    static_cast<CProjectile*>(pObj)->Set_Owner(m_pTarget);//총알의 주인을 총의 소유자로 설정한다.
}

void CChargeBlasterLight::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // image.png 파일을 이용하여 Image 객체를 생성합니다.

    Graphics Grapics(hDC);

    // 회전의 중심점 설정 (이미지의 중심)
    int centerX = int(m_tRect.left + iScrollX + 64 * 0.5f);
    int centerY = int(m_tRect.top + iScrollY + 64 * 0.5f);

    // 회전 변환 적용
    Grapics.TranslateTransform((REAL)centerX, (REAL)centerY);  // 회전 중심으로 이동
    Grapics.RotateTransform(-m_fAngle);        // 회전 각도 적용
    Grapics.TranslateTransform((REAL)-centerX, (REAL)-centerY); // 원래 위치로 이동

    Grapics.DrawImage(m_pImage, m_tRect.left + iScrollX, m_tRect.top + iScrollY, 64, 64);


    //무기 출력
    /*HDC hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"ChargeBlasterLight");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        64, 64,
        hTestDC, 0, 0, 64, 64,
        RGB_WHITE);*/
}
