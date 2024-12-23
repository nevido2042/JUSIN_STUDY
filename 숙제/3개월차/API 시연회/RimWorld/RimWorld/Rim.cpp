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
#include "TimeMgr.h"

CRim::CRim()
    :m_bTaskCheck(false)
{

}

CRim::~CRim()
{
    Release();
}

void CRim::Initialize()
{
    CPawn::Initialize();

    m_fSpeed = 2.f;

    m_fMaxHP = 100.f;
    m_fHP = m_fMaxHP;

    //���� ����
    m_pRangedWeapon = CAbstractFactory<CBoltActionRifle>::Create(m_tInfo.fX, m_tInfo.fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WEAPON, m_pRangedWeapon);
    m_pRangedWeapon->Set_Target(this);

    Take_Damage(10.f);

    Change_State(WANDERING);
}

int CRim::Update()
{
    return CPawn::Update();
}

void CRim::Late_Update()
{
    CPawn::Late_Update();

    //���콺�� �ö����ִ°�?
    if (Is_MouseHovered_Scrolled())
    {
        //��Ŭ���� Ÿ������ ����
        if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
        {
            CColonyMgr::Get_Instance()->Set_Target(this);
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
        Ellipse(hDC, int(pNode->Get_Pos().iX + iScrollX - 10.f), int(pNode->Get_Pos().iY + iScrollY - 10.f),
            int(pNode->Get_Pos().iX + 10.f + iScrollX), int(pNode->Get_Pos().iY + 10.f + iScrollY));
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

    switch (Get_State())
    {
    case CRim::WANDERING:
        wsprintf(buffer, L"m_eState: %s", L"WANDERING");
        break;
    case CRim::DRAFTED:
        wsprintf(buffer, L"m_eState: %s", L"DRAFTED");
        break;
    case CRim::UNDRAFTED:
        wsprintf(buffer, L"m_eState: %s", L"UNDRAFTED");
        break;
    case CRim::CONSTRUCTING:
        wsprintf(buffer, L"m_eState: %s", L"CONSTRUCTING");
        break;
    case CRim::DECONSTRUCTING:
        wsprintf(buffer, L"m_eState: %s", L"DECONSTRUCTING");
        break;
    case CRim::END:
        break;
    default:
        break;
    }
    // ���ڿ� ��� (�����ڵ�)
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY + 40), buffer, lstrlenW(buffer));

}

void CRim::Handle_Wandering()
{
    //��ȸ ����
    Wander();
    //���ο� �۾��� ����ٸ�?????????? �۾��� ������ �޶����ٸ�?
    //�۾� ����� �޶����ٸ�? �۾�����Ʈ�� ���� ���翡�� ������ �ִ´�?
    // 
    //####�ݷδ� �Ŵ������� �۾�����Ʈ �ٲ���ٰ� �˷��ָ�?<�̰� �´µ�?>
    //####�ݷδ� �Ŵ������� �۾��� �ٲ���� ��(�߰�, ����) ��� ������, �۾� Ȯ���϶� ����
    
    //�۾��� üũ�� �ð��� ��ٸ�(�̰� ��������)
    if (m_bTaskCheck)
    {
        Check_ConstructWork();
        Check_DeconstructWork();
        //m_fElapsedTimeCheck = 0.f;
        m_bTaskCheck = false;
    }
}

void CRim::Handle_Drafted()
{
    if (m_pTarget)
    {
        //Ÿ���� ������ Ÿ�� ���ֱ�
        CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
        if (pTarget->Get_IsDead())
        {
            Set_Target(nullptr);
            //���� �׾����� �ٸ� ���� ã�´�.
            //Find_Enemy();
        }
        else
        {
            //���缭 ���� ���� �� ��ã���ؾ���!!!!!!!!!!!
            //Move_To(POS{ m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY });
        }
    }
    else
    {
        Find_Enemy();
    }

    //�����Ÿ��� �ְ� ���� ���δٸ�
    if (IsWithinRange() && IsCanSeeTarget())
    {
        static_cast<CRangedWeapon*>(m_pRangedWeapon)->Fire();
        m_bAttack = true;
    }
    else
    {
        m_bAttack = false;
    }

}

void CRim::Handle_Undrafted()
{

}

void CRim::Handle_Deconstructing()
{
    //Ÿ���� ������� Ȯ��
    if (m_fTargetDist < TILECX * 1.5f)
    {
        //������ �����.
        //RequestNavStop();
        m_bNavigating = false;
    }
    //�ح��µ�
    if (!m_bNavigating)
    {
        //Ÿ���� ����.
        if (!m_pTarget)
        {
            return;
        }
        //Ÿ���� �ִ�
        if (m_fTargetDist > TILECX * 1.5f)
        {
            //�ٸ� �۾� üũ
            m_bTaskCheck = true;
            Change_State(WANDERING);
            return;
        }
        //��ü �ϴ°�
        Deconstruct();
    }

    //��ü �������� �� ����
    //���� �� ��ü �Ϸ�
}

void CRim::Handle_Constructing()
{
    //Ÿ���� ������� Ȯ��
    if (m_fTargetDist < TILECX * 1.5f)
    {
        //������ �����.
        //RequestNavStop();
        m_bNavigating = false;
    }
    //�ح��µ�
    if (!m_bNavigating)
    {
        //Ÿ���� ����.
        if (!m_pTarget)
        {
            return;
        }
        //Ÿ���� �ִ�
        if (m_fTargetDist > TILECX * 1.5f)
        {
            //�ٸ� �۾� üũ
            m_bTaskCheck = true;
            m_pTarget = nullptr;
            Change_State(WANDERING);
            return;
        }
        //�Ǽ�
        Construct();
    }



    //��ü �������� �� ����
    //���� �� ��ü �Ϸ�
}

void CRim::Check_CloseTask()
{
}

void CRim::Deconstruct()
{
    CSteelWall* pWall = static_cast<CSteelWall*>(m_pTarget);
    pWall->Set_IsBrokenDown();
    Change_State(WANDERING);
}

void CRim::Construct()
{
    CObj* pObj = CAbstractFactory<CSteelWall>::Create(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
    CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, OPT_BLOCKED);
    CTileMgr::Get_Instance()->Set_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, pObj);

    //�۾�����
    //CColonyMgr::Get_Instance()->Get_ConstructSet()->erase(m_pTarget);
    set<TASK>& ConstructSet = *CColonyMgr::Get_Instance()->Get_ConstructSet();
    for (auto Iter = ConstructSet.begin(); Iter != ConstructSet.end();)
    {
        if ((*Iter).pObj == m_pTarget)
        {
            Iter = ConstructSet.erase(Iter);
        }
        else
        {
            ++Iter;
        }
    }

    m_pTarget = nullptr;
    Change_State(WANDERING);
}

void CRim::Check_DeconstructWork()
{
    //�Ĺ��� �����ڿ� ��ü�� ������ �ִ��� Ȯ��         //�׸��� �� ���󰡴� ���̾ƴϰ�, �۾��������� �ƴҶ� �� , ���ο� �۾��� ������ �� �˻�
    if (!CColonyMgr::Get_Instance()->Get_DeconstructSet()->empty() && Get_State() == WANDERING) //�̰� ���� ���μ������� �ſ� ���� �ϸ� �ɵ�?
    {
        //��ü�� ���� �� ���� ã�� �� �ִ� ���� ������
        //�ش� ���� �ֺ��� 8���� Ÿ���� Ȯ���ؼ� ���� ã�� �� �ִ��� Ȯ��
        //���� ��ã���� ��ü��������, ���� ã���� ��ü�Ϸ�����

        //Set�� vector�� ������ ����
        set<TASK>& DeconstructSet = *CColonyMgr::Get_Instance()->Get_DeconstructSet();
        vector<TASK> vecDeconstruct(DeconstructSet.begin(), DeconstructSet.end());
        // ����� ���� ����: ���������� �Ÿ��� ����� ����
        std::sort(vecDeconstruct.begin(), vecDeconstruct.end(), 
            [this](const TASK _tTaskA, const TASK _tTaskB) 
            {
                // ������� ������ �켱���� ������
                if (!_tTaskA.pRimReserved && _tTaskB.pRimReserved)
                {
                    return true;  // _tTaskA�� �� �տ� ������
                }

                if (_tTaskA.pRimReserved && !_tTaskB.pRimReserved)
                {
                    return false; // _tTaskB�� �� �տ� ������
                }

                float fDistA = CObj::Calculate_Dist(this, _tTaskA.pObj);
                float fDistB = CObj::Calculate_Dist(this, _tTaskB.pObj);
                return fDistA < fDistB; // �Ÿ��� �������� �������� ����
            });

        //�۾��ؾ��� ���� Ž��(������ ���� ����� �༮��� ����)
        for (TASK _tTask : vecDeconstruct)
        {
            //���� ��� ����Ʈ
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();

            //�̵� ������ Ÿ���� ������ ��帮��Ʈ ��ȯ
            m_NodeList = move(CTileMgr::Get_Instance()
                ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)_tTask.pObj->Get_Info().fX, (int)_tTask.pObj->Get_Info().fY }));

            if (m_NodeList.empty())
            {
                continue;
            }

            Set_Target(_tTask.pObj);
            //�۾� ����Ʈ���� �� �۾��� ���������� ǥ��
            for (auto Iter = DeconstructSet.begin(); Iter != DeconstructSet.end();)
            {
                if ((*Iter).pObj == m_pTarget)
                {
                    //���� ��
                    Iter = DeconstructSet.erase(Iter);
                    //�����ؼ� �߰�
                    TASK tTask;
                    tTask.pObj = m_pTarget;
                    tTask.pRimReserved = this;
                    DeconstructSet.emplace(tTask);
                }
                else
                {
                    ++Iter;
                }
            }

            m_bNavigating = true;
            Change_State(DECONSTRUCTING);
            return;
            //Move_To(POS{ pTile->Get_Info().fX,pTile->Get_Info().fX });
        }
    }
}

void CRim::Check_ConstructWork()
{
    //�Ĺ��� �����ڿ� ��ü�� ������ �ִ��� Ȯ��         //�׸��� �� ���󰡴� ���̾ƴϰ�, �۾��������� �ƴҶ� �� , ���ο� �۾��� ������ �� �˻�
    if (!CColonyMgr::Get_Instance()->Get_ConstructSet()->empty() && Get_State() == WANDERING) //�̰� ���� ���μ������� �ſ� ���� �ϸ� �ɵ�?
    {
        //��ü�� ���� �� ���� ã�� �� �ִ� ���� ������
        //�ش� ���� �ֺ��� 8���� Ÿ���� Ȯ���ؼ� ���� ã�� �� �ִ��� Ȯ��
        //���� ��ã���� ��ü��������, ���� ã���� ��ü�Ϸ�����

        //Set�� vector�� ������ ����
        set<TASK>& ConstructSet = *CColonyMgr::Get_Instance()->Get_ConstructSet();

        //�ش� Ÿ�� ���� ö �������� ������ ������.

        vector<TASK> vecConstruct(ConstructSet.begin(), ConstructSet.end());

        // ����� ���� ����: ���������� �Ÿ��� ����� ����
        std::sort(vecConstruct.begin(), vecConstruct.end(), 
            [this](const TASK _tTaskA, const TASK _tTaskB)
            {
                // ������� ������ �켱���� ������
                if (!_tTaskA.pRimReserved && _tTaskB.pRimReserved)
                {
                    return true;  // _tTaskA�� �� �տ� ������
                }

                if (_tTaskA.pRimReserved && !_tTaskB.pRimReserved)
                {
                    return false; // _tTaskB�� �� �տ� ������
                }

                float fDistA = CObj::Calculate_Dist(this, _tTaskA.pObj);
                float fDistB = CObj::Calculate_Dist(this, _tTaskB.pObj);
                return fDistA < fDistB; // �Ÿ��� �������� �������� ����
            });

        //�۾��ؾ��� ���� Ž��(������ ���� ����� �༮��� ����)
        for (TASK tTask : vecConstruct)
        {
            //���� ��� ����Ʈ
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();

            //�̵� ������ Ÿ���� ������ ��帮��Ʈ ��ȯ
            m_NodeList = move(CTileMgr::Get_Instance()
                ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)tTask.pObj->Get_Info().fX, (int)tTask.pObj->Get_Info().fY }));

            if (m_NodeList.empty())
            {
                continue;
            }

            Set_Target(tTask.pObj);
            //�۾� ����Ʈ���� �� �۾��� ���������� ǥ��
            for (auto Iter = ConstructSet.begin(); Iter != ConstructSet.end();)
            {
                if ((*Iter).pObj == m_pTarget)
                {
                    //���� ��
                    Iter = ConstructSet.erase(Iter);
                    //�����ؼ� �߰�
                    TASK tTask;
                    tTask.pObj = m_pTarget;
                    tTask.pRimReserved = this;
                    ConstructSet.emplace(tTask);
                }
                else
                {
                    ++Iter;
                }
            }
            m_bNavigating = true;
            Change_State(CONSTRUCTING);
            return;
            //Move_To(POS{ pTile->Get_Info().fX,pTile->Get_Info().fX });
        }
    }
}

void CRim::Find_Enemy()
{
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ENEMY])
    {
        if (static_cast<CPawn*>(pObj)->Get_IsDead())
        {
            return;
        }

        Set_Target(pObj);
        Measure_Target();
        if (IsWithinRange())
        {
            break;
        }
        else
        {
            Set_Target(nullptr);
        }
    }
}

