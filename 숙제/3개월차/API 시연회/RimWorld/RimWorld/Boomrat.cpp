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

    Take_Damage(99.f);

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

        // �� ����, ��, �Ӹ� ��
    HDC hTestDC(nullptr);
    switch (m_eDir)
    {
    case UU:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_north");
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX
            , m_tRect.top + iScrollY,
            64, 64,
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_east");
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX
            , m_tRect.top + iScrollY,
            64, 64,
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_south");
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX
            , m_tRect.top + iScrollY,
            64, 64,
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_west");
        // ����
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

    //�� ã�� ��� ���
    for (CNode* pNode : m_NodeList)
    {
        Ellipse(hDC, int(pNode->Get_Pos().fX + iScrollX - 10.f), int(pNode->Get_Pos().fY + iScrollY - 10.f),
            int(pNode->Get_Pos().fX + 10.f + iScrollX), int(pNode->Get_Pos().fY + 10.f + iScrollY));
    }

    //Ÿ�� ���
    if (m_pTarget)
    {
        // ������ �� ����
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // ���� 1, ������
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);       // ���� �� ���� �� ������ �� ����

        int iX = (int)m_pTarget->Get_Info().fX;
        int iY = (int)m_pTarget->Get_Info().fY;

        Ellipse(hDC, int(iX + iScrollX - 10), int(iY + iScrollY - 10),
            int(iX + 10 + iScrollX), int(iY + 10 + iScrollY));

        // �� ����
        SelectObject(hDC, hOldBrush); // ���� �� ����
        DeleteObject(hBrush);         // ������ �� ����
    }
    if (!m_pTarget)
    {
        return;
    }

    // ���� ���� �����ڵ� ���ڿ��� ��ȯ
    wchar_t buffer[50];
    wsprintf(buffer, L"m_bAttack: %d", m_bAttack);
    // ���ڿ� ��� (�����ڵ�)
    if (!m_pTarget)
    {
        return;
    }
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY), buffer, lstrlenW(buffer));
    // ���� ���� �����ڵ� ���ڿ��� ��ȯ
    wsprintf(buffer, L"m_pTarget: %p (%d, %d)", m_pTarget, (int)m_pTarget->Get_Info().fX / TILECX, (int)m_pTarget->Get_Info().fY / TILECY);
    // ���ڿ� ��� (�����ڵ�)
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY + 20), buffer, lstrlenW(buffer));
}

void CBoomrat::Dead()
{
    CPawn::Dead();
    Boom();
}

void CBoomrat::Handle_Wandering()
{
    Wander();
    //FindTarget();
    //Ÿ���� ���� ���
    //�� ������Ʈ����Ʈ���� ���� ����� ���� ã�� Ÿ������ ����
    Find_Target();

    if (m_pTarget)
    {
        Change_State(CHASING);
    }
}

void CBoomrat::Handle_Chasing()
{
    if (!m_pTarget)
    {
        Change_State(WANDERING);
    }

    //Ÿ�� ������ ���󰡱�
    if (m_pTarget)
    {
        POS tMoveToPos{ m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY };

        //Ÿ���� Pawn�̶��
        if (CPawn* pPawnTarget = dynamic_cast<CPawn*>(m_pTarget))//Ÿ���� Pawn�̰� �׾�����
        {
            //Pawn�� �׾��ٸ�
            if (pPawnTarget->Get_IsDead())
            {
                Set_Target(nullptr);
                RequestNavStop();
            }
        }
        Move_To(tMoveToPos);
    }

    //�����Ÿ� ���� �ְ�, ���� ��������
    if (IsWithinRange() && IsCanSeeTarget())
    {
        RequestNavStop();
        //static_cast<CRangedWeapon*>(m_pRangedWeapon)->Fire();
        m_bAttack = true;
        //����
        Boom();
    }
    else
    {
        m_bAttack = false;
    }
}

void CBoomrat::Handle_Deconstructing()
{
    if (!Get_Target())
    {
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
            ->Find_ReachableTiles(POS{ m_tInfo.fX,m_tInfo.fY },
                POS{ m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY }));

        if (!m_NodeList.empty())
        {
            m_bNavigating = true;
        }
    }

    //Ÿ���� ������� Ȯ��
    if (m_fTargetDist < TILECX * 1.5f)
    {
        //������ �����.
        RequestNavStop();

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

void CBoomrat::Find_Target()
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

void CBoomrat::Boom()
{
    CSoundMgr::Get_Instance()->StopSound(SOUND_EXPLOSION);
    CSoundMgr::Get_Instance()->PlaySound(L"GiantExplosion.wav", SOUND_EXPLOSION, 0.5f);

    //���� ���� ��ŭ
    //üũ�ؼ�
    //�������� �ش�.
    //�ڱ� �ڽ��� ���δ�.
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
        //        // �ι� ���̸� ���� ���׳�
        //        continue;
        //    }
        //}

        if (fExplosionRadius > fDist)
        {
            static_cast<CPawn*>(pObj)->Take_Damage(m_fMaxHP);
        }
    }
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ENEMY])
    {
        float fDist = CObj::Calculate_Dist(this, pObj);

        if (fExplosionRadius > fDist)
        {
            static_cast<CSteelWall*>(pObj)->Take_Damage(m_fMaxHP);
        }
    }

}
