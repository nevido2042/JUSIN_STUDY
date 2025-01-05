#include "pch.h"
#include "Enemy.h"
#include "RangedWeapon.h"
#include "ObjMgr.h"
#include "TileMgr.h"
#include "SteelWall.h"
#include "TimeMgr.h"

CEnemy::CEnemy()
    :m_fDestroyTime(0.f), m_fDestroyTime_Elapsed(0.f)
{
}

CEnemy::~CEnemy()
{
    Release();
}

void CEnemy::Initialize()
{
    CPawn::Initialize();

    m_fDestroyTime = 1000.f;
}

int CEnemy::Update()
{
    if (m_bDead)
    {
        m_fDestroyTime_Elapsed += GAMESPEED;
    }

    if (m_fDestroyTime < m_fDestroyTime_Elapsed)
    {
        Set_Destroyed();
    }

    return CPawn::Update();
}

void CEnemy::Late_Update()
{
    CPawn::Late_Update();
}

void CEnemy::Render(HDC hDC)
{
}

void CEnemy::Handle_Wandering()
{
    Wander();

    Find_Target();

    if (m_pTarget)
    {
        Change_State(CHASING, m_pTarget);
    }
}

void CEnemy::Handle_Chasing()
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

void CEnemy::Handle_Deconstructing()
{
    if (!Get_Target())
    {
        Change_State(WANDERING);
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
    if (m_fTargetDist < TILECX * 1.5f)
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

void CEnemy::Find_Target()
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
        if (!pTarget || fDist < fMinDist)
        {
            pTarget = pObj;
            fMinDist = fDist;
        }
    }

    Set_Target(pTarget);
}
