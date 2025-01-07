#include "pch.h"
#include "Projectile.h"
#include "TimeMgr.h"
#include "Pawn.h"
#include "SteelWall.h"
#include "SoundMgr.h"

CProjectile::CProjectile()
    :m_fDamage(0.f), m_pOwner(nullptr)
{
}

CProjectile::~CProjectile()
{
    Release();
}

void CProjectile::Initialize()
{
    m_tInfo.fCX = 10.f;
    m_tInfo.fCY = 10.f;

    m_eRenderID = RENDER_GAMEOBJECT;

    m_fSpeed = 20.f;

    m_fDamage = 7.f;
}

int CProjectile::Update()
{
    return CObj::Update();
}

void CProjectile::Late_Update()
{
    //Angle방향대로 날라간다.
    float fRadian = m_fAngle * (PI / 180.f);

    m_tInfo.fX += m_fSpeed * cosf(fRadian) * GAMESPEED;
    m_tInfo.fY -= m_fSpeed * sinf(fRadian) * GAMESPEED;
}

void CProjectile::Render(HDC hDC)
{
    //int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    //int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    //화면 스크롤 값을 가져옵니다.
    //int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    //int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // 화면 확대/축소 비율을 가져옵니다.
    //float fZoom = CCamera::Get_Instance()->Get_Zoom();

    // 월드 좌표를 스크린 좌표로 변환합니다.
    POINT tScreenPosLeft = CCamera::Get_Instance()->WorldToScreen(m_tRect.left, m_tRect.top);
    POINT tScreenPosRight = CCamera::Get_Instance()->WorldToScreen(m_tRect.right, m_tRect.bottom);

    // Ellipse에 대한 위치와 크기 보정
    int left = (int)(tScreenPosLeft.x);  // 왼쪽 상단 위치 보정
    int top = (int)(tScreenPosLeft.y);  // 위쪽 상단 위치 보정
    int right = (int)((tScreenPosRight.x));  // 오른쪽 위치 크기 보정
    int bottom = (int)((tScreenPosRight.y));  // 아래쪽 위치 크기 보정

    // 엘립스 그리기
    Ellipse(hDC,
        left,
        top,
        right,
        bottom
    );

}

void CProjectile::Release()
{
}

void CProjectile::OnCollision(OBJID _eID, CObj* _pOther)
{
    //총알을 쏜 주인이 부딫히면 무시한다.
    if (m_pOwner == _pOther)
    {
        return;
    }
    //이미 죽은 애면 무시한다.
    if (static_cast<CPawn*>(_pOther)->Get_IsDead())
    {
        return;
    }
    //림이거나, 메카노이드면 데미지주고 없어진다.
    if (_eID == OBJ_RIM || _eID == OBJ_ENEMY)
    {
        if (_eID == OBJ_RIM)
        {
            //CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
            CSoundMgr::Get_Instance()->PlaySound(L"Bullet_Flesh_1a.wav", SOUND_FLESH, .3f);
        }
        else
        {
            //CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
            CSoundMgr::Get_Instance()->PlaySound(L"Bullet_Metal_1g.wav", SOUND_METAL, .05f);
        }

        static_cast<CPawn*>(_pOther)->Take_Damage(m_fDamage);
        Set_Destroyed();
    }
    else if (_eID == OBJ_WALL)
    {
        static_cast<CSteelWall*>(_pOther)->Take_Damage(m_fDamage);
        Set_Destroyed();
    }
}
