#include "pch.h"
#include "BoltActionRifle.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"

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

    //총 발사 관련
    m_fFireRate = 100.f;
    m_fReloadSpeed = 1.f;
    //무기 사정거리
    m_fRange = 500.f;

    m_pImage = Gdiplus::Image::FromFile(L"../Image/Things/Weapon/BoltActionRifle.png");
}

void CBoltActionRifle::Fire()
{
    if (!Try_Fire())
    {
        return;
    }

    CSoundMgr::Get_Instance()->StopSound(SOUND_BOLTACTION);//총 소리 내기전에 전에 나던 총소리 꺼버리기
    CSoundMgr::Get_Instance()->PlaySound(L"GunShotA.wav", SOUND_BOLTACTION, 0.5f);

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
            if (pTargetRim->Get_State() != CRim::DRAFTED && pTargetRim->Get_State() != CRim::CHASING)
            {
                return;
            }
        }
    }

    POINT tPoint = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left, (float)m_tRect.top);
    float fZoom = CCamera::Get_Instance()->Get_Zoom();
    Graphics Grapics(hDC);

    POINT tCenterPoint = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left + 64 * 0.5f, (float)m_tRect.top + 64 * 0.5f);
    // 회전의 중심점 설정 (이미지의 중심)
    int centerX = tCenterPoint.x;// int(tPoint.x + iScrollX + 64 * 0.5f);
    int centerY = tCenterPoint.y;// int(tPoint.y + iScrollY + 64 * 0.5f);

    // 회전 변환 적용
    Grapics.TranslateTransform((REAL)centerX, (REAL)centerY);  // 회전 중심으로 이동
    Grapics.RotateTransform(-m_fAngle);                       // 회전 각도 적용

    // 상하 반전 적용
    if (270.f > m_fAngle && 90.f < m_fAngle) // 왼쪽을 바라보는 경우 플래그
    {
        Grapics.ScaleTransform(1.0f, -1.0f); // Y축을 반전
    }

    // 원래 위치로 이동
    Grapics.TranslateTransform((REAL)-centerX, (REAL)-centerY);

    // 이미지 그리기
    Grapics.DrawImage(m_pImage, tPoint.x, tPoint.y, int(64 * fZoom), int(64 * fZoom));

}
