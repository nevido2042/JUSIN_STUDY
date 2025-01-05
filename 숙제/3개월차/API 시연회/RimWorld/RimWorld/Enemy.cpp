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

    //Ÿ�� ������ ���󰡱�
    if (m_pTarget)
    {
        POS tMoveToPos{ (int)m_pTarget->Get_Info().fX, (int)m_pTarget->Get_Info().fY };

        //Ÿ���� Pawn�̶��
        if (CPawn* pPawnTarget = dynamic_cast<CPawn*>(m_pTarget))//Ÿ���� Pawn�̰� �׾�����
        {
            //Pawn�� �׾��ٸ�
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

    //�����Ÿ� ���� �ְ�, ���� ��������
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

    //�� ��ֹ���ó �ö� �� �ִ� Ÿ���� �ִ��� Ȯ���Ѵ�.
    //Ȯ�� �� ���� ã�� �� �ִ��� Ȯ���Ѵ�.
    //���� ã���� ���󰡴ٰ� ��������� �μ���.
    //�̵� ������ Ÿ���� ������ ��帮��Ʈ ��ȯ

    if (!m_bNavigating)
    {
        //���� ��� ����Ʈ
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

    //Ÿ���� ������� Ȯ��
    if (m_fTargetDist < TILECX * 1.5f)
    {
        //������ �����.
        //RequestNavStop();
        m_bNavigating = false;

        //Ÿ���� ����.
        if (!m_pTarget)
        {
            return;
        }
        //���� �μ���
        CSteelWall* pWall = static_cast<CSteelWall*>(m_pTarget);
        pWall->Set_IsBrokenDown();
        //Ÿ���� ã�´�.
        Find_Target();
        //���� ���·� �����.
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
        //���� ���̸� ��Ƽ��
        CPawn* pPawn = static_cast<CPawn*>(pObj);
        if (pPawn->Get_IsDead())
        {
            continue;
        }

        float fWidth(0.f);
        float fHeight(0.f);
        float fDist(0.f);
        //�Ÿ� ���
        fWidth = pObj->Get_Info().fX - Get_Info().fX;
        fHeight = pObj->Get_Info().fY - Get_Info().fY;
        fDist = sqrtf(fWidth * fWidth + fHeight * fHeight);
        //Ÿ���� ���ų�, �Ÿ��� �� ����� �ָ� �߰��ϸ� Ÿ�� ����
        if (!pTarget || fDist < fMinDist)
        {
            pTarget = pObj;
            fMinDist = fDist;
        }
    }

    Set_Target(pTarget);
}
