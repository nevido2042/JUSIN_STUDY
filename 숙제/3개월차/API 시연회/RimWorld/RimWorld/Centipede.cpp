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

    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_fSpeed = 1.f;

    m_eRenderID = RENDER_GAMEOBJECT;

    //���� ����
    m_pRangedWeapon = CAbstractFactory<CChargeBlasterLight>::Create(m_tInfo.fX, m_tInfo.fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WEAPON, m_pRangedWeapon);
    m_pRangedWeapon->Set_Target(this);

    Take_Damage(50.f);

    //Find_Target();
}

int CCentipede::Update()
{
    if (m_bDestroyed)
        return OBJ_DESTROYED;

    //�׾����� ����
    if (m_bDead)
    {
        return OBJ_NOEVENT;
    }

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CCentipede::Late_Update()
{
    CPawn::Late_Update();

    //�����Ÿ� ���� �ְ�, ���� ��������
    if (IsWithinRange() && IsCanSeeTarget())
    {
        RequestNavStop();
        static_cast<CRangedWeapon*>(m_pRangedWeapon)->Fire();
        m_bAttack = true;
    }
    else
    {
        m_bAttack = false;
    }
    //FindTarget();
    //Ÿ���� ���� ���
    //�� ������Ʈ����Ʈ���� ���� ����� ���� ã�� Ÿ������ ����
    if (!m_pTarget)
    {
        Change_State(CHASING);
        Find_Target();
    }

    //���콺 Ŭ�� ���� �� Ÿ������ ����
    if (Is_MouseHovered())
    {
        //��Ŭ���� Ÿ���� ���� Ÿ������ ����
        if (CKeyMgr::Get_Instance()->Key_Up(VK_RBUTTON))
        {
            if (CObj* pTarget = CColonyMgr::Get_Instance()->Get_Target())
            {
                pTarget->Set_Target(this);
            }
            return;
        }
    }
    
}

void CCentipede::Render(HDC hDC)
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
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_north");
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_east");
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_south");
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_west");
        // ����
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
    TextOutW(hDC, int(m_tInfo.fX+ iScrollX), int(m_tInfo.fY + iScrollY), buffer, lstrlenW(buffer));
    // ���� ���� �����ڵ� ���ڿ��� ��ȯ
    wsprintf(buffer, L"m_pTarget: %p (%d, %d)", m_pTarget, (int)m_pTarget->Get_Info().fX/TILECX, (int)m_pTarget->Get_Info().fY / TILECY);
    // ���ڿ� ��� (�����ڵ�)
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY + 20), buffer, lstrlenW(buffer));
}

void CCentipede::Handle_Wandering()
{
}

void CCentipede::Handle_Chasing()
{
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
}

void CCentipede::Handle_Deconstructing()
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

void CCentipede::Find_Target()
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
        if (!pTarget||fDist < fMinDist)
        {
            pTarget = pObj;
            fMinDist = fDist;
        }
    }

    Set_Target(pTarget);
}
