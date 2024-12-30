#include "pch.h"
#include "Centipede.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "KeyMgr.h"
#include "ColonyMgr.h"
#include "SteelWall.h"
#include "TileMgr.h"

CCentipede::CCentipede()
{
}

CCentipede::~CCentipede()
{
	Release();
}

void CCentipede::Initialize()
{
    CPawn::Initialize();

    m_fSpeed = 1.f;

    m_fMaxHP = 300.f;
    m_fHP = m_fMaxHP;

    //무기 생성
    m_pRangedWeapon = CAbstractFactory<CChargeBlasterLight>::Create(m_tInfo.fX, m_tInfo.fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WEAPON, m_pRangedWeapon);
    m_pRangedWeapon->Set_Target(this);

    Take_Damage(50.f);

    Change_State(WANDERING);
}

int CCentipede::Update()
{
    return CPawn::Update();
}

void CCentipede::Late_Update()
{
    CPawn::Late_Update();
}

void CCentipede::Render(HDC hDC)
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
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_north");
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_east");
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_south");
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_west");
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
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
    TextOutW(hDC, int(m_tInfo.fX+ iScrollX), int(m_tInfo.fY + iScrollY), buffer, lstrlenW(buffer));
    // 변수 값을 유니코드 문자열로 변환
    wsprintf(buffer, L"m_pTarget: %p (%d, %d)", m_pTarget, (int)m_pTarget->Get_Info().fX/TILECX, (int)m_pTarget->Get_Info().fY / TILECY);
    // 문자열 출력 (유니코드)
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY + 20), buffer, lstrlenW(buffer));
}

void CCentipede::Handle_Wandering()
{
    Wander();

    Find_Target();

    if (m_pTarget)
    {
        Change_State(CHASING);
    }
}

void CCentipede::Handle_Chasing()
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
        static_cast<CRangedWeapon*>(m_pRangedWeapon)->Fire();
        m_bAttack = true;
    }
    else
    {
        m_bAttack = false;
    }
}

void CCentipede::Handle_Deconstructing()
{
    if (!Get_Target())
    {
        return;
    }

    //그 장애물근처 올라갈 수 있는 타일이 있는지 확인한다.
    //확인 후 길을 찾을 수 있는지 확인한다.
    //길을 찾으면 따라가다가 가까워지면 부순다.
    //이동 가능한 타일이 있으면 노드리스트 반환

    if (!m_bNavigating)
    {
        //기존 노드 딜리트
        for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
        m_NodeList.clear();

        m_NodeList = move(CTileMgr::Get_Instance()
            ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY },
                POS{ (int)m_pTarget->Get_Info().fX, (int)m_pTarget->Get_Info().fY }));

        if (!m_NodeList.empty())
        {
            m_bNavigating = true;
        }
    }

    //타겟이 가까운지 확인
    if (m_fTargetDist < TILECX * 1.2f)
    {
        //가까우면 멈춘다.
        //RequestNavStop();
        m_bNavigating = false;

        //타겟이 없다.
        if (!m_pTarget)
        {
            return;
        }
        //벽을 부수고
        CSteelWall* pWall = static_cast<CSteelWall*>(m_pTarget);
        pWall->Set_IsBrokenDown();
        //타겟을 찾는다.
        Find_Target();
        //추적 상태로 만든다.
        Change_State(CHASING);
    }
}

void CCentipede::Find_Target()
{
    list<CObj*> RimList = CObjMgr::Get_Instance()->Get_List()[OBJ_RIM];
    float fMinDist(0.f);
    CObj* pTarget(nullptr);

    for (CObj* pObj : RimList)
    {
        //죽은 놈이면 컨티뉴
        CPawn* pPawn = static_cast<CPawn*>(pObj);
        if (pPawn->Get_IsDead())
        {
            continue;
        }

        float fWidth(0.f);
        float fHeight(0.f);
        float fDist(0.f);
        //거리 계산
        fWidth = pObj->Get_Info().fX - Get_Info().fX;
        fHeight = pObj->Get_Info().fY - Get_Info().fY;
        fDist = sqrtf(fWidth * fWidth + fHeight * fHeight);
        //타겟이 없거나, 거리가 더 가까운 애를 발견하면 타겟 변경
        if (!pTarget||fDist < fMinDist)
        {
            pTarget = pObj;
            fMinDist = fDist;
        }
    }

    Set_Target(pTarget);
}
