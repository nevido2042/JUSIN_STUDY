#include "pch.h"
#include "Boomrat.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "KeyMgr.h"
#include "ColonyMgr.h"
#include "SteelWall.h"
#include "TileMgr.h"
#include "SoundMgr.h"
#include "EffectMgr.h"

CBoomrat::CBoomrat()
{
}

CBoomrat::~CBoomrat()
{
    Release();
}

void CBoomrat::Initialize()
{
    CEnemy::Initialize();

    m_fSpeed = 1.5f;

    m_fMaxHP = 10.f;
    m_fHP = m_fMaxHP;

    //Take_Damage(99.f);

    Change_State(WANDERING);
}

int CBoomrat::Update()
{
    return CEnemy::Update();
}

void CBoomrat::Late_Update()
{
    CPawn::Late_Update();
}

void CBoomrat::Render(HDC hDC)
{
    // 화면 스크롤 값을 가져옵니다.
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // 화면 확대/축소 비율을 가져옵니다.
    float fZoom = CCamera::Get_Instance()->Get_Zoom();

    // 월드 좌표를 스크린 좌표로 변환합니다.
    POINT tScreenPos = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left, (float)m_tRect.top);

    // 림 몸통, 얼굴, 머리 순
    HDC hTestDC(nullptr);
    switch (m_eDir)
    {
    case UU:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_north");
        // 몸통
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x),  // 위치 보정만 하고 fZoom은 크기 보정에만 사용
            (int)(tScreenPos.y),
            (int)(64 * fZoom),  // 크기 보정 (fZoom만 크기 조정)
            (int)(64 * fZoom),
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_east");
        // 몸통
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x),  // 위치 보정만 하고 fZoom은 크기 보정에만 사용
            (int)(tScreenPos.y),
            (int)(64 * fZoom),  // 크기 보정 (fZoom만 크기 조정)
            (int)(64 * fZoom),
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_south");
        // 몸통
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x),  // 위치 보정만 하고 fZoom은 크기 보정에만 사용
            (int)(tScreenPos.y),
            (int)(64 * fZoom),  // 크기 보정 (fZoom만 크기 조정)
            (int)(64 * fZoom),
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_west");
        // 몸통
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x),  // 위치 보정만 하고 fZoom은 크기 보정에만 사용
            (int)(tScreenPos.y),
            (int)(64 * fZoom),  // 크기 보정 (fZoom만 크기 조정)
            (int)(64 * fZoom),
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    default:
        break;
    }

}

void CBoomrat::Dead()
{
    CPawn::Dead();
    CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
    CSoundMgr::Get_Instance()->PlaySound(L"Boomrat_Death_1b.wav", SOUND_EFFECT, .1f);
    Boom();
}

void CBoomrat::Handle_Chasing()
{
    if (!m_pTarget)
    {
        Change_State(WANDERING);
    }

    //타겟 있으면 따라가기
    if (m_pTarget)
    {
        POS tMoveToPos{ (int)m_pTarget->Get_Info().fX, (int)m_pTarget->Get_Info().fY };

        //타겟이 Pawn이라면
        if (CPawn* pPawnTarget = dynamic_cast<CPawn*>(m_pTarget))//타겟이 Pawn이고 죽었으면
        {
            //Pawn이 죽었다면
            if (pPawnTarget->Get_IsDead())
            {
                Set_Target(nullptr);
                //RequestNavStop();
                m_bNavigating = false;
            }
        }

        if (!m_bNavigating)
        {
            Move_To(tMoveToPos);
        }
    }

    //사정거리 내에 있고, 적이 보여야함
    if (IsWithinRange() && IsCanSeeTarget())
    {
        //RequestNavStop();
        m_bNavigating = false;
        //static_cast<CRangedWeapon*>(m_pRangedWeapon)->Fire();
        m_bAttack = true;
        //자폭
        Boom();
    }
    else
    {
        m_bAttack = false;
    }
}

void CBoomrat::Boom()
{
    //폭발 이펙트
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            POS tPos{ (int)m_tInfo.fX + j * TILECX, (int)m_tInfo.fY + i * TILECY };
            CEffectMgr::Get_Instance()->Create_Effect(tPos, 64.f, 64.f, L"BlastFlame", 2.f);
        }
    }

    //폭발 사운드
    CSoundMgr::Get_Instance()->StopSound(SOUND_EXPLOSION);
    CSoundMgr::Get_Instance()->PlaySound(L"GiantExplosion.wav", SOUND_EXPLOSION, 0.5f);

    //폭발 범위 만큼
    //체크해서
    //데미지를 준다.
    //자기 자신을 죽인다.
    float fExplosionRadius(100.f);
    //float fDamage(100.f);

    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_RIM])
    {
        float fDist =  CObj::Calculate_Dist(this, pObj);

        if (fExplosionRadius > fDist)
        {
            static_cast<CPawn*>(pObj)->Take_Damage(m_fMaxHP);
        }
    }
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ENEMY])
    {
        float fDist = CObj::Calculate_Dist(this, pObj);

        //if (pObj == this)
        //{
        //    if (static_cast<CPawn*>(pObj)->Get_IsDead())
        //    {
        //        // 두번 죽이면 사운드 버그남
        //        continue;
        //    }
        //}

        if (fExplosionRadius > fDist)
        {
            static_cast<CPawn*>(pObj)->Take_Damage(m_fMaxHP);
        }
    }
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_WALL])
    {
        float fDist = CObj::Calculate_Dist(this, pObj);

        if (fExplosionRadius > fDist)
        {
            static_cast<CSteelWall*>(pObj)->Take_Damage(m_fMaxHP);
        }
    }

}
