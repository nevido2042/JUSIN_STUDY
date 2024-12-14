#include "pch.h"
#include "Centipede.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"

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

    m_fSpeed = 3.f;

    m_eRenderID = RENDER_GAMEOBJECT;

    Take_Damage(50.f);
}

int CCentipede::Update()
{
    if (m_bDestroyed)
        return OBJ_DESTROYED;

    if (m_bNavigating)
    {
        Navigate();
    }
    else
    {
        if (m_pTarget)
        {
            Move_To(POS{ m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY });

        }
    }

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CCentipede::Late_Update()
{
    Calculate_MoveDir();
    //FindTarget();
    //Ÿ���� ���� ���
    //�� ������Ʈ����Ʈ���� ���� ����� ���� ã�� Ÿ������ ����
    if (!m_pTarget)
    {
        Find_Target();
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
}

void CCentipede::Find_Target()
{
    list<CObj*> RimList = CObjMgr::Get_Instance()->Get_List()[OBJ_RIM];
    float fMinDist(0.f);
    CObj* pTarget(nullptr);

    for (CObj* pObj : RimList)
    {
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

void CCentipede::Dead()
{
}
