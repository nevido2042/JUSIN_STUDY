#include "pch.h"
#include "Rim.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ColonyMgr.h"
#include "KeyMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "TileMgr.h"
#include "PathFinder.h"
#include "SteelWall.h"

CRim::CRim()
    :m_eState(END), m_bFindingTask(false)
{

}

CRim::~CRim()
{
    Release();
}

void CRim::Change_State(STATE _eState)
{
    m_eState = _eState;  
    m_pTarget = nullptr; 
    m_bFindingTask = true;
}

void CRim::Initialize()
{
    CPawn::Initialize();

    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_fSpeed = 2.f;

    m_eRenderID = RENDER_GAMEOBJECT;

    //���� ����
    m_pRangedWeapon = CAbstractFactory<CBoltActionRifle>::Create(m_tInfo.fX, m_tInfo.fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WEAPON, m_pRangedWeapon);
    m_pRangedWeapon->Set_Target(this);

    Take_Damage(10.f);

    m_eState = UNDRAFTED; //����� ���������� ���� �⺻�� �����ൿ����
}

int CRim::Update()
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

void CRim::Late_Update()
{
    //�׾����� ����
    if (m_bDead)
    {
        return;
    }

    //�̵����� ���
    Calculate_MoveDir();

    if (m_pTarget)
    {
        //Ÿ�ٰ��� �Ÿ� �� ���� ���
        Measure_Target();
    }

    switch (m_eState)
    {
    case CRim::DRAFTED:
        Drafed();
        break;
    case CRim::UNDRAFTED:
        Undrafed();
        break;
    case CRim::WORKING:
        Work();
        break;
    default:
        break;
    }

    if (m_bNavigating)
    {
        Navigate();
    }

    //���콺�� �ö����ִ°�?
    if (Is_MouseHovered())
    {
        //��Ŭ���� Ÿ������ ����
        if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
        {
            CColonyMgr::Get_Instance()->Set_Target(this);
            return;
        }
        //��Ŭ���� Ÿ���� ���� Ÿ������ ����
        else if (CKeyMgr::Get_Instance()->Key_Up(VK_RBUTTON))
        {
            if (CObj* pTarget = CColonyMgr::Get_Instance()->Get_Target())
            {
                //�ڱ� �ڽ��� Ÿ���� �� �� ����.
                if (pTarget != this)
                {
                    pTarget->Set_Target(this);
                }
            }
            return;
        }
    }

    
               
}

void CRim::Render(HDC hDC)
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
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_north");
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //��
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_north");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // �Ӹ�
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Afro_north");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_east");
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //��
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_east");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // �Ӹ�
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Afro_east");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_south");
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //��
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_south");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // �Ӹ�
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Afro_south");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_west");
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //��
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_west");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // �Ӹ�
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Afro_west");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;
    default:
        break;
    }

    //�׾��� �� ���ڱ� ���
    if (m_bDead)
    {
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Blood");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX,
            m_tRect.top + iScrollY,
            64, 64,
            hTestDC, 0, 0, 64, 64,
            RGB_WHITE);
    }
    

    //�� ã�� ��� ���
    for (CNode* pNode : m_NodeList)
    {
        Ellipse(hDC, int(pNode->Get_Pos().fX + iScrollX - 10.f), int(pNode->Get_Pos().fY + iScrollY - 10.f),
            int(pNode->Get_Pos().fX + 10.f + iScrollX), int(pNode->Get_Pos().fY + 10.f + iScrollY));
    }

    // ���� ���� �����ڵ� ���ڿ��� ��ȯ
    wchar_t buffer[50];
    wsprintf(buffer, L"m_bAttack: %d", m_bAttack);
    // ���ڿ� ��� (�����ڵ�)
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY), buffer, lstrlenW(buffer));
    // ���� ���� �����ڵ� ���ڿ��� ��ȯ
    wsprintf(buffer, L"m_pTarget: %p", m_pTarget);
    // ���ڿ� ��� (�����ڵ�)
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY + 20), buffer, lstrlenW(buffer));

}

void CRim::Deconstruct()
{
    m_pTarget->Set_Destroyed();
    m_eState = UNDRAFTED;
}

void CRim::Drafed()
{
    //���� �Ÿ����� �ְ� ������ ���� �ʾҴٸ�
    if (IsWithinRange())
    {
        //RequestNavStop();
        m_bAttack = true;
    }
    else
    {
        m_bAttack = false;
    }

    //Ÿ�� ������ ���󰡱�
    if (m_pTarget)
    {
        //Ÿ���� ������ Ÿ�� ���ֱ�
        CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
        if (pTarget->Get_IsDead())
        {
            Set_Target(nullptr);
        }
        else
        {
            //���缭 ���� ���� �� ��ã���ؾ���!!!!!!!!!!!
            Move_To(POS{ m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY });
        }
    }

}

void CRim::Undrafed()
{
    //���ο� �۾��� �߰� ��ٸ�
    if (CColonyMgr::Get_Instance()->Get_NewTaskAdded())
    {
        //�۾��� ã�ƶ�
        m_bFindingTask = true;
    }

    //�Ĺ��� �����ڿ� ��ü�� ������ �ִ��� Ȯ��         //�׸��� �� ���󰡴� ���̾ƴϰ�, �۾��������� �ƴҶ� �� , ���ο� �۾��� ������ �� �˻�
    if (!CColonyMgr::Get_Instance()->Get_DeconstructSet()->empty() && m_eState == UNDRAFTED && m_bFindingTask) //�̰� ���� ���μ������� �ſ� ���� �ϸ� �ɵ�?
    {
        //��ü�� ���� �� ���� ã�� �� �ִ� ���� ������
        //�ش� ���� �ֺ��� 8���� Ÿ���� Ȯ���ؼ� ���� ã�� �� �ִ��� Ȯ��
        //���� ��ã���� ��ü��������, ���� ã���� ��ü�Ϸ�����


        //Set�� vector�� ������ ����
        set<CObj*>& DeconstructSet = *CColonyMgr::Get_Instance()->Get_DeconstructSet();

        vector<CObj*> vecDeconstruct(DeconstructSet.begin(), DeconstructSet.end());


        // ����� ���� ����: ���������� �Ÿ��� ����� ����
        std::sort(vecDeconstruct.begin(), vecDeconstruct.end(), [this](CObj* _Dst, CObj* _Src) {
            float fDistA = CObj::Calculate_Dist(this, _Dst);
            float fDistB = CObj::Calculate_Dist(this, _Src);
            return fDistA < fDistB; // �Ÿ��� �������� �������� ����
            });

        //�۾��ؾ��� ���� Ž��(������ ���� ����� �༮��� ����)
        for (CObj* pWall : vecDeconstruct)
        {
            //�̵� ������ Ÿ���� ������ ��帮��Ʈ ��ȯ
            m_NodeList = move(CTileMgr::Get_Instance()
                ->Find_ReachableTiles(POS{ m_tInfo.fX,m_tInfo.fY }, POS{ pWall->Get_Info().fX, pWall->Get_Info().fY }));

            if (m_NodeList.empty())
            {
                continue;
            }

            Set_Target(pWall);
            m_bNavigating = true;
            m_eState = WORKING;
            return;
            //Move_To(POS{ pTile->Get_Info().fX,pTile->Get_Info().fX });
        }

        //�۾� �Ҽ� �ִ� ���� ��ã��.
        m_bFindingTask = false;

    }
}

void CRim::Work()
{
    //Ÿ���� ������� Ȯ��
    if (m_fTargetDist < TILECX * 1.5f)
    {
        RequestNavStop();
        CSteelWall* pWall = static_cast<CSteelWall*>(m_pTarget);
        pWall->Set_IsBrokenDown();
    }
    //Ÿ���� ������ ��ü ����
    if (!m_bNavigating)
    {
        Deconstruct();
    }
    //��ü �������� �� ����
    //���� �� ��ü �Ϸ�
}

