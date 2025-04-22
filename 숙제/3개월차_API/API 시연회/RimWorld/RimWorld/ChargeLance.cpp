#include "pch.h"
#include "ChargeLance.h"
#include "BmpMgr.h"

#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

CChargeLance::CChargeLance()
{
}

CChargeLance::~CChargeLance()
{
    Release();
}

void CChargeLance::Initialize()
{
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_WEAPON;

    //총 발사 관련
    m_fFireRate = 100.f;
    m_fReloadSpeed = 1.f;
    //무기 사정거리
    m_fRange = 300.f;

    m_pImage = Gdiplus::Image::FromFile(L"../Image/Things/Weapon/ChargeLance.png");
}

void CChargeLance::Fire()
{
    if (!Try_Fire())
    {
        return;
    }

    CSoundMgr::Get_Instance()->StopSound(SOUND_CHARGELANCE);
    CSoundMgr::Get_Instance()->PlaySound(L"ChargeLance_Fire_4a.wav", SOUND_CHARGELANCE, .2f);

    //각도 랜덤
    int iRand = (rand() % 21) - 10; //-10 ~ 10

    CObj* pObj = CAbstractFactory<CCharge_Small>::Create(m_tInfo.fX, m_tInfo.fY, float(m_fAngle + iRand));
    CObjMgr::Get_Instance()->Add_Object(OBJ_PROJECTILE, pObj);
    static_cast<CProjectile*>(pObj)->Set_Owner(m_pTarget);//총알의 주인을 총의 소유자로 설정한다.
}

void CChargeLance::Render(HDC hDC)
{
    CRangedWeapon::Render(hDC);
}
