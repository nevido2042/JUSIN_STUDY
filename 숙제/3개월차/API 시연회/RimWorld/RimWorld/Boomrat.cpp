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
    CPawn::Initialize();

    m_fSpeed = 1.5f;

    m_fMaxHP = 10.f;
    m_fHP = m_fMaxHP;

    //Take_Damage(99.f);

    Change_State(WANDERING);
}

int CBoomrat::Update()
{
    return CPawn::Update();
}

void CBoomrat::Late_Update()
{
    CPawn::Late_Update();
}

void CBoomrat::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    /*Ellipse(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
        m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);*/

        // 림 몸통, 얼굴, 머리 순
    HDC hTestDC(nullptr);
    switch (m_eDir)
    {
    case UU:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_north");
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX
            , m_tRect.top + iScrollY,
            64, 64,
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_east");
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX
            , m_tRect.top + iScrollY,
            64, 64,
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_south");
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX
            , m_tRect.top + iScrollY,
            64, 64,
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_west");
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX
            , m_tRect.top + iScrollY,
            64, 64,
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;
    default:
        break;
    }

    //길 찾기 노드 출력
    for (CNode* pNode : m_NodeList)
    {
        Ellipse(hDC, int(pNode->Get_Pos().iX + iScrollX - 10.f), int(pNode->Get_Pos().iY + iScrollY - 10.f),
            int(pNode->Get_Pos().iX + 10.f + iScrollX), int(pNode->Get_Pos().iY + 10.f + iScrollY));
    }

    //타겟 출력
    if (m_pTarget)
    {
        // 빨간색 펜 생성
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // 굵기 1, 빨간색
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);       // 기존 펜 저장 및 빨간색 펜 설정

        int iX = (int)m_pTarget->Get_Info().fX;
        int iY = (int)m_pTarget->Get_Info().fY;

        Ellipse(hDC, int(iX + iScrollX - 10), int(iY + iScrollY - 10),
            int(iX + 10 + iScrollX), int(iY + 10 + iScrollY));

        // 펜 정리
        SelectObject(hDC, hOldBrush); // 기존 펜 복원
        DeleteObject(hBrush);         // 빨간색 펜 삭제
    }
    if (!m_pTarget)
    {
        return;
    }

    // 변수 값을 유니코드 문자열로 변환
    wchar_t buffer[50];
    wsprintf(buffer, L"m_bAttack: %d", m_bAttack);
    // 문자열 출력 (유니코드)
    if (!m_pTarget)
    {
        return;
    }
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY), buffer, lstrlenW(buffer));
    // 변수 값을 유니코드 문자열로 변환
    wsprintf(buffer, L"m_pTarget: %p (%d, %d)", m_pTarget, (int)m_pTarget->Get_Info().fX / TILECX, (int)m_pTarget->Get_Info().fY / TILECY);
    // 문자열 출력 (유니코드)
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY + 20), buffer, lstrlenW(buffer));
}

void CBoomrat::Dead()
{
    CPawn::Dead();
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
