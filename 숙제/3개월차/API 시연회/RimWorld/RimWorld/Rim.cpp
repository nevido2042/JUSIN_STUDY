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
#include "SoundMgr.h"
#include "Ship.h"
#include "Campfire.h"
#include "Camera.h"

CRim::CRim()
    :m_bTaskCheck(false), m_pTransportingItem(nullptr),
    m_fTaskCheckTime(0.f), m_fTaskCheckTime_Elapsed(0.f),
    m_eDraftState(DRAFTED_END)
{

}

CRim::~CRim()
{
    Release();
}

void CRim::Initialize()
{
    CPawn::Initialize();

    m_fTaskCheckTime = 100.f;

    m_fSpeed = 2.f;

    m_fMaxHP = 100.f;
    m_fHP = m_fMaxHP;

    //���� ����
    m_pRangedWeapon = CAbstractFactory<CBoltActionRifle>::Create(m_tInfo.fX, m_tInfo.fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WEAPON, m_pRangedWeapon);
    m_pRangedWeapon->Set_Target(this);

    //Take_Damage(10.f);

    Change_State(WANDERING);

    //ĳ���� �̹��� ����
    //m_pImgKey_Body = L"Naked_Female"; //L"Naked_Male";
    //m_pImgKey_Face = L"Female_Average_Normal";//L"Male_Average_Normal";
    //m_pImgKey_Hair = L"Decent"; //L"Afro";

    int iRand(0);
    iRand = rand() % 3;
    // �������� Body �̹��� ����
    if (iRand == 0)
    {
        m_pImgKey_Body = L"Naked_Female";
    }
    else if(iRand == 1)
    {
        m_pImgKey_Body = L"Naked_Male";
    }
    else if (iRand == 2)
    {
        m_pImgKey_Body = L"Naked_Fat";
    }

    // �������� Face �̹��� ����
    if (rand() % 2 == 0) 
    {
        m_pImgKey_Face = L"Female_Average_Normal";
    }
    else 
    {
        m_pImgKey_Face = L"Male_Average_Normal";
    }

    // �������� Hair �̹��� ����
    iRand = rand() % 3;
    if (iRand == 0)
    {
        m_pImgKey_Hair = L"Decent";
    }
    else if(iRand == 1)
    {
        m_pImgKey_Hair = L"Afro";
    }
    else if (iRand == 2)
    {
        m_pImgKey_Hair = L"Pigtails";
    }

    m_ImgKeyArr_Body[NORTH] = m_pImgKey_Body + L"_north";
    m_ImgKeyArr_Body[EAST] = m_pImgKey_Body + L"_east";
    m_ImgKeyArr_Body[SOUTH] = m_pImgKey_Body + L"_south";
    m_ImgKeyArr_Body[WEST] = m_pImgKey_Body + L"_west";

    m_ImgKeyArr_Face[NORTH] = m_pImgKey_Face + L"_north";
    m_ImgKeyArr_Face[EAST] = m_pImgKey_Face + L"_east";
    m_ImgKeyArr_Face[SOUTH] = m_pImgKey_Face + L"_south";
    m_ImgKeyArr_Face[WEST] = m_pImgKey_Face + L"_west";

    m_ImgKeyArr_Hair[NORTH] = m_pImgKey_Hair + L"_north";
    m_ImgKeyArr_Hair[EAST] = m_pImgKey_Hair + L"_east";
    m_ImgKeyArr_Hair[SOUTH] = m_pImgKey_Hair + L"_south";
    m_ImgKeyArr_Hair[WEST] = m_pImgKey_Hair + L"_west";
}

int CRim::Update()
{
    return CPawn::Update();
}

void CRim::Late_Update()
{
    CPawn::Late_Update();
}

void CRim::Render(HDC hDC)
{

    CPawn::Render(hDC);

    /*Ellipse(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
        m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);*/

    // �� ����, ��, �Ӹ� ��
    HDC hTestDC(nullptr);


    switch (m_eDir) 
    {
    case UU:
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Body[NORTH], IMAGE_OFFSET_X, IMAGE_OFFSET_Y); // ����
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Face[NORTH], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // ��
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Hair[NORTH], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // �Ӹ�
        break;

    case RR:
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Body[EAST], IMAGE_OFFSET_X, IMAGE_OFFSET_Y); // ����
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Face[EAST], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // ��
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Hair[EAST], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // �Ӹ�
        break;

    case DD:
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Body[SOUTH], IMAGE_OFFSET_X, IMAGE_OFFSET_Y); // ����
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Face[SOUTH], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // ��
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Hair[SOUTH], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // �Ӹ�
        break;

    case LL:
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Body[WEST], IMAGE_OFFSET_X, IMAGE_OFFSET_Y); // ����
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Face[WEST], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // ��
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Hair[WEST], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // �Ӹ�
        break;

    default:
        break;
    }


    //�׾��� �� ���ڱ� ���
    if (m_bDead)
    {
        POINT tWorldPoint = CCamera::Get_Instance()->WorldToScreen(float(m_tRect.left), float(m_tRect.top));
        float fZoom = CCamera::Get_Instance()->Get_Zoom();
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Blood");
        GdiTransparentBlt(hDC,
            tWorldPoint.x,// m_tRect.left + iScrollX,
            tWorldPoint.y,//m_tRect.top + iScrollY,
            int(64* fZoom), int(64* fZoom),
            hTestDC, 0, 0, 64, 64,
            RGB_WHITE);
    }
}

void CRim::Handle_Wandering()
{
    m_fTaskCheckTime_Elapsed += GAMESPEED;

    if (m_fTaskCheckTime < m_fTaskCheckTime_Elapsed)
    {
        m_bTaskCheck = true;
        m_fTaskCheckTime_Elapsed = 0.f;
    }

    //�� ��� ���� �ʵ���
    if (m_pTransportingItem)
    {
        PutDown_Item();
    }

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
        //�Ǽ��� �տ� ����, ��ᰡ ��� transport ���°� �Ǽ� ��ü(?), ������ ���Ѵ�
        Check_ConstructWork();
        //������ �߿���
        Check_DeconstructWork();
        Check_LoggingWork();

        //Check_TransportingWork();
        //m_fElapsedTimeCheck = 0.f;
        m_bTaskCheck = false;
    }
}

void CRim::Handle_Drafted()
{
    //�� Ž�� ����
    //�� ���� ����
    //�� ��� ���� ����

    switch (m_eDraftState)
    {
    case CRim::DRAFTED_SEARCH:
        Drfated_Search();
        break;
    case CRim::DRAFHTED_AIM:
        Drfated_Aim();
        break;
    case CRim::DRAFTED_FIRE:
        Drfated_Fire();
        break;
    case CRim::DRAFTED_END:
        break;
    default:
        break;
    }


    //if (m_pTarget)
    //{
    //    //�����Ÿ��� �ְ� ���� ���δٸ�
    //    if (IsWithinRange() && IsCanSeeTarget())
    //    {
    //        static_cast<CRangedWeapon*>(m_pRangedWeapon)->Fire();
    //        m_bAttack = true;
    //    }
    //    else
    //    {
    //        Set_Target(nullptr);
    //        m_bAttack = false;
    //    }
    //}

    //if (m_pTarget)
    //{
    //    //Ÿ���� ������ Ÿ�� ���ֱ�
    //    CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
    //    if (pTarget->Get_IsDead())
    //    {
    //        Set_Target(nullptr);
    //        //���� �׾����� �ٸ� ���� ã�´�.
    //        //Find_Enemy();
    //    }
    //}
    //else
    //{
    //    if (!m_bNavigating)
    //    {
    //        Find_Enemy();
    //    }
    //}

}

void CRim::Handle_Undrafted()
{

}

void CRim::Handle_Deconstructing()
{
    if (!m_pTarget)
    {
        m_bTaskCheck = true;
        Change_State(WANDERING);
        return;
    }

    Deconstruct();
}

void CRim::Handle_Constructing()
{
    //�Ǽ� �Ҷ� �̹� ���� ������ ö �������� �ٸ� �� ã�ƶ�
    if (CTileMgr::Get_Instance()->Get_TileOption(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY) == OPT_BLOCKED)
    {
        m_bTaskCheck = true;
        Change_State(WANDERING);
        PutDown_Item();
        return;
    }

    //�ű�� ö�� ����.
    if (!m_pTransportingItem)
    {
        m_bTaskCheck = true;
        Change_State(WANDERING);
        return;
    }

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

void CRim::Handle_Transporting()
{
    //ó�����ö� Ÿ���� ���°� ����
    //Ÿ�� �������� ��������� �������� ����
    if (m_pTarget)
    {
        if (!m_pTransportingItem && m_fTargetDist < TILECX * 1.5f)
        {

            PickUp_Item(m_pTarget);

        }
    }

    //���� �Ǽ��۾��� ���� �ľ��ϰ�, � ��Ḧ ������������ �Ǵ�
    if (!m_pTarget)
    {
        CObj* pItem(nullptr);

        if (m_eCurrentTask.eType == TASK::CAMPFIRE)
        {
            //����
            if (!m_pTransportingItem)
            {
                pItem = Find_Item(L"WoodLog_b");

                if (pItem)
                {
                    Set_Target(pItem);
                }
                else
                {
                    //m_bTaskCheck = true;
                    Change_State(WANDERING);
                    return;
                }
            }
        }
        else
        {
            //ö
            if (!m_pTransportingItem)
            {
                pItem = Find_Item(L"Steel_b");

                if (pItem)
                {
                    Set_Target(pItem);
                }
                else
                {
                    m_bTaskCheck = true;
                    Change_State(WANDERING);
                    return;
                }
            }
        }
    }

    if (m_pTarget && !m_bNavigating)
    {
        POS tPos{ (int)m_pTarget->Get_Info().fX, (int)m_pTarget->Get_Info().fY };
        Move_To(tPos);
    }


    //Ÿ������ �� ö�� �ٸ� �ְ� �̹� ����������
    if (m_pTarget)
    {
        CObj* pOwner = m_pTarget->Get_Target();
        if (pOwner && pOwner != this)
        {
            m_bTaskCheck = true;
            m_pTarget = nullptr;
            return;
        }
    }

    //�������� ������� �Ǽ��Ϸ�����
    if (m_pTransportingItem)
    {
        Change_State(MOVETOWORK, m_eCurrentTask.pObj);
    }
}

void CRim::Handle_Boarding()
{
    if (!m_bNavigating)
    {
        int iTileIndex = CTileMgr::Get_TileIndex(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
        int iReachableTileIndex = Find_NearestReachableTile(iTileIndex % TILEX, iTileIndex / (TILEX));

        POS tPos{ (int)((iReachableTileIndex % TILEX) * TILECX + TILECX * 0.5f),
            (int)((iReachableTileIndex / TILEX) * TILECY + TILECY * 0.5f) };

        Move_To(tPos);
    }
    
    //Ÿ��(���ּ�)�� �Ÿ��� ��������� ž��
    if (m_fTargetDist < TILECX * 2.f)
    {
        m_bNavigating = false;
        Set_Pos(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
    }

}

void CRim::Handle_Logging()
{
    if (!m_pTarget)
    {
        m_bTaskCheck = true;
        Change_State(WANDERING);
        return;
    }

    Log();
}

void CRim::Handle_MoveToWork()
{
    if (m_pTarget)
    {
        if (!m_bNavigating)
        {
            //���� ��� ����Ʈ
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();

            //�̵� ������ Ÿ���� ������ ��帮��Ʈ ��ȯ
            m_NodeList = move(CTileMgr::Get_Instance()
                ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)m_pTarget->Get_Info().fX, (int)m_pTarget->Get_Info().fY }));

            if (m_NodeList.empty())
            {
                return;
            }

            m_bNavigating = true;
        }

        if (CBreakable* pWall = dynamic_cast<CBreakable*>(m_pTarget))
        {
            if (pWall->Get_IsBrokenDown())
            {
                //�ٸ� �۾� üũ
                m_bTaskCheck = true;
                Change_State(WANDERING);
                return;
            }
        }
        //Ÿ���� �ı����.
        if (m_pTarget->Get_Destroyed())
        {
            //�ٸ� �۾� üũ
            m_bTaskCheck = true;
            Change_State(WANDERING);
            return;
        }
    }
    else
    {
        //�ٸ� �۾� üũ
        m_bTaskCheck = true;
        Change_State(WANDERING);
        return;
    }

    //Ÿ���� ������� Ȯ��
    if (m_fTargetDist < TILECX * 1.5f)
    {
        //������ �����.
        //RequestNavStop();
        m_bNavigating = false;

        if (m_pTarget->Get_ObjID() == OBJ_TREE)
        {
            Change_State(LOGGING, m_pTarget);
        }
        else if(m_pTarget->Get_ObjID() == OBJ_WALL)
        {
            Change_State(DECONSTRUCTING, m_pTarget);
        }
        else if (m_pTarget->Get_ObjID() == OBJ_TILE)
        {
            Change_State(CONSTRUCTING, m_pTarget);
        }
    }
}

void CRim::Change_State(STATE _eState, CObj* _pTarget)
{
    CPawn::Change_State(_eState, _pTarget);

    if (_eState == STATE::DRAFTED)
    {
        m_eDraftState = DRAFTED_SEARCH;
    }
}

void CRim::Change_DraftedState(DRAFTED_STATE _eState)
{
    if (m_eDraftState == _eState)
    {
        return;
    }

    m_eDraftState = _eState;
}

void CRim::Drfated_Search()
{
     Find_Enemy();

     if (m_pTarget)
     {
         Change_DraftedState(DRAFHTED_AIM);
     }
}

void CRim::Drfated_Aim()
{
    if (m_pTarget)
    {
        //Ÿ���� ������ Ÿ�� ���ֱ�
        CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
        if (pTarget->Get_IsDead())
        {
            Set_Target(nullptr);
            //���� �׾����� �ٸ� ���� ã�´�.
            Change_DraftedState(DRAFTED_SEARCH);
        }
    }


    if (m_pTarget)
    {
        //�����Ÿ��� �ְ� ���� ���δٸ�
        if (IsWithinRange() && IsCanSeeTarget())
        {
            Change_DraftedState(DRAFTED_FIRE);
        }
    }
    else
    {
        Change_DraftedState(DRAFTED_SEARCH);
    }
}

void CRim::Drfated_Fire()
{
    if (m_bNavigating)
    {
        m_bAttack = false;
        return;
    }

    if (m_pTarget)
    {
        //Ÿ���� ������ Ÿ�� ���ֱ�
        CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
        if (pTarget->Get_IsDead())
        {
            Set_Target(nullptr);
            //���� �׾����� �ٸ� ���� ã�´�.
            m_bAttack = false;
            Change_DraftedState(DRAFTED_SEARCH);
        }
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
        Change_DraftedState(DRAFTED_SEARCH);
    }
}

void CRim::DrawImage(HDC hDC, const RECT& m_tRect, const std::wstring& imageKey, int offsetX, int offsetY)
{

    POINT tPoint = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left, (float)m_tRect.top);

    // �̹��� �׸��� �Լ�
        HDC hTestDC = CBmpMgr::Get_Instance()->Find_Image(imageKey.c_str());
        GdiTransparentBlt(hDC,
            tPoint.x - int(offsetX * CCamera::Get_Instance()->Get_Zoom()),
            tPoint.y - int(offsetY * CCamera::Get_Instance()->Get_Zoom()),
            int(128 * CCamera::Get_Instance()->Get_Zoom()), int(128 * CCamera::Get_Instance()->Get_Zoom()),
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
    
}

void CRim::Check_CloseTask()
{
}

void CRim::Deconstruct()
{
    if (m_fMeleeAttackSpeed > m_fMeleeElapsed)
    {
        return;
    }

    m_fMeleeElapsed = 0.f;

    CSoundMgr::Get_Instance()->StopSound(SOUND_DECONSTRUCT);
    CSoundMgr::Get_Instance()->PlaySound(L"PickHitA.wav", SOUND_DECONSTRUCT, .5f);

    CBreakable* pWall = static_cast<CBreakable*>(m_pTarget);
    //pWall->Set_IsBrokenDown();
    //Change_State(WANDERING);

    //�ش� ���� ������ �� ���� ���� �ݺ�
    //�ð� �����ϴ� �ð� ���ؾ���

    pWall->Take_Damage(1.f);

}

void CRim::Construct()
{
    //ö ����
    //CObj* pSteel =  CTileMgr::Get_Instance()->Get_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
    //pSteel->Set_Destroyed();

    //�̹� Ÿ�� ���� ���� ������ ��� ��Ű��
    if (CTileMgr::Get_Instance()->Get_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY))
    {
        m_bTaskCheck = true;
        PutDown_Item();
        Change_State(WANDERING);
        return;
    }

    //��ü �ð� �ɸ��� �ϰ� ���� ���
    if (m_fConstructTime > m_fConstructElapsed)
    {
        switch (m_eCurrentTask.eType)
        {
        case TASK::TYPE::CAMPFIRE:
            //�����Ҹ� ����
            //CSoundMgr::Get_Instance()->StopSound(SOUND_CONSTRUCT_WOOD);
            CSoundMgr::Get_Instance()->PlaySound(L"Hammer_Nail_Wood_1a.wav", SOUND_CONSTRUCT_WOOD, .5f);
            break;
        case TASK::TYPE::WALL:
        case TASK::TYPE::SHIP:
            //�帱�Ҹ� ����
            //CSoundMgr::Get_Instance()->StopSound(SOUND_CONSTRUCT_STEEL);
            CSoundMgr::Get_Instance()->PlaySound(L"DrillB.wav", SOUND_CONSTRUCT_STEEL, .5f);
            break;
        }


        m_fConstructElapsed += GAMESPEED;
        return;
    }

    m_fConstructElapsed = 0.f;

    CSoundMgr::Get_Instance()->StopSound(SOUND_CONSTRUCT_FINISH);
    CSoundMgr::Get_Instance()->PlaySound(L"BuildingComplete.wav", SOUND_CONSTRUCT_FINISH, .5f);

    //��� �ִ� ö�� ����
    m_pTransportingItem->Set_Destroyed();
    m_pTransportingItem = nullptr;

    if (m_eCurrentTask.eType == TASK::SHIP)
    {
        if (CTutorialMgr::Get_Instance()->Get_CurQuest() == CTutorialMgr::QUEST_CONSTRUCT_SHIP)
        {
            CTutorialMgr::Get_Instance()->IncreaseQuestProgress();
        }

        //���ּ� �Ǽ�
        CObj* pObj = CAbstractFactory<CShip>::Create(m_pTarget->Get_Info().fX + TILECX * 0.5f, m_pTarget->Get_Info().fY + TILECY * 0.5f);
        CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
        CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX + TILECX, m_pTarget->Get_Info().fY, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX + TILECX, m_pTarget->Get_Info().fY + TILECY, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY + TILECY, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, pObj);
    }
    else if (m_eCurrentTask.eType == TASK::WALL)
    {
        if (CTutorialMgr::Get_Instance()->Get_CurQuest() == CTutorialMgr::QUEST::QUEST_CONSTRUCT_WALL)
        {
            CTutorialMgr::Get_Instance()->IncreaseQuestProgress();
        }

        //�� �Ǽ�
        CObj* pObj = CAbstractFactory<CSteelWall>::Create(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
        CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
        CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, pObj);
    }
    else if (m_eCurrentTask.eType == TASK::CAMPFIRE)
    {
        if (CTutorialMgr::Get_Instance()->Get_CurQuest() == CTutorialMgr::QUEST::QUEST_CONSTRUCT_CAMPFIRE)
        {
            CTutorialMgr::Get_Instance()->IncreaseQuestProgress();
        }

        //��ں� �Ǽ�
        CObj* pObj = CAbstractFactory<CCampfire>::Create(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
        CObjMgr::Get_Instance()->Add_Object(OBJ_CAMPFIRE, pObj);
        CTileMgr::Get_Instance()->Set_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, pObj);
    }

    //�۾�����
    //CColonyMgr::Get_Instance()->Get_ConstructSet()->erase(m_pTarget);
    set<TASK>& ConstructSet = *CColonyMgr::Get_Instance()->Get_ConstructSet();
    for (auto Iter = ConstructSet.begin(); Iter != ConstructSet.end();)
    {
        if ((*Iter).pObj == m_pTarget)
        {
            if (CTile* pTile = dynamic_cast<CTile*>(Iter->pObj))
            {
                pTile->Set_Reserved(CTile::RESERVED_END);
            }

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

void CRim::Log()
{
    if (m_fMeleeAttackSpeed > m_fMeleeElapsed)
    {
        return;
    }

    m_fMeleeElapsed = 0.f;

    CSoundMgr::Get_Instance()->StopSound(SOUND_DECONSTRUCT);
    CSoundMgr::Get_Instance()->PlaySound(L"Tree_Chop_1a.wav", SOUND_DECONSTRUCT, .5f);

    CBreakable* pTree = static_cast<CBreakable*>(m_pTarget);
    //pWall->Set_IsBrokenDown();
    //Change_State(WANDERING);

    //�ش� ���� ������ �� ���� ���� �ݺ�
    //�ð� �����ϴ� �ð� ���ؾ���

    pTree->Take_Damage(1.f);

    if (pTree->Get_IsBrokenDown())
    {
        m_pTarget = nullptr;
    }

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
            ////���� ��� ����Ʈ
            //for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            //m_NodeList.clear();

            ////�̵� ������ Ÿ���� ������ ��帮��Ʈ ��ȯ
            //m_NodeList = move(CTileMgr::Get_Instance()
            //    ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)_tTask.pObj->Get_Info().fX, (int)_tTask.pObj->Get_Info().fY }));

            //if (m_NodeList.empty())
            //{
            //    continue;
            //}

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
            Change_State(MOVETOWORK, _tTask.pObj);
            //Set_Target(_tTask.pObj);
            return;
            //Move_To(POS{ pTile->Get_Info().fX,pTile->Get_Info().fX });
        }
    }
}

void CRim::Check_ConstructWork()
{
    //�Ĺ��� �����ڿ� ��ü�� ������ �ִ��� Ȯ��         //�׸��� �� ���󰡴� ���̾ƴϰ�, �۾��������� �ƴҶ� �� , ���ο� �۾��� ������ �� �˻�
    if (!CColonyMgr::Get_Instance()->Get_ConstructSet()->empty() && (Get_State() == WANDERING|| Get_State() == TRANSPORTING)) //�̰� ���� ���μ������� �ſ� ���� �ϸ� �ɵ�?
    {


        //��ü�� ���� �� ���� ã�� �� �ִ� ���� ������
        //�ش� ���� �ֺ��� 8���� Ÿ���� Ȯ���ؼ� ���� ã�� �� �ִ��� Ȯ��
        //���� ��ã���� ��ü��������, ���� ã���� ��ü�Ϸ�����

        //Set�� vector�� ������ ����
        set<TASK>& ConstructSet = *CColonyMgr::Get_Instance()->Get_ConstructSet();
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
            //for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            //m_NodeList.clear();

            //�̵� ������ Ÿ���� ������ ��帮��Ʈ ��ȯ
            list<CNode*> NodeList = move(CTileMgr::Get_Instance()
                ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)tTask.pObj->Get_Info().fX, (int)tTask.pObj->Get_Info().fY }));

            if (NodeList.empty())
            {
                continue;
            }

            for_each(NodeList.begin(), NodeList.end(), Safe_Delete<CNode*>);
            NodeList.clear();

            //��ᰡ �ִ��� Ȯ��
            CObj* pItem(nullptr);
            switch (tTask.eType)
            {
            case TASK::CAMPFIRE:
                pItem = Find_Item(L"WoodLog_b");
                break;
            case TASK::SHIP:
                pItem = Find_Item(L"Steel_b");
                break;
            case TASK::WALL:
                pItem = Find_Item(L"Steel_b");
                break;
            default:
                break;
            }

            if (!pItem)
            {
                continue;
            }


            Set_Target(tTask.pObj);
            //�۾� ����Ʈ���� �� �۾��� ���������� ǥ��
            for (auto Iter = ConstructSet.begin(); Iter != ConstructSet.end();)
            {
                if ((*Iter).pObj == m_pTarget)
                {
                    TASK tTas = *Iter;
                    //���� ��
                    Iter = ConstructSet.erase(Iter);
                    //�����ؼ� �߰�
                    //tTask.pObj = m_pTarget;
                    tTask.pRimReserved = this;
                    //tTask.eType = TASK::WALL;
                    ConstructSet.emplace(tTask);
                }
                else
                {
                    ++Iter;
                }
            }
            m_bNavigating = true;
            m_eCurrentTask = tTask;
            Change_State(TRANSPORTING);
            //Set_Target(tTask.pObj);
            return;
            //Move_To(POS{ pTile->Get_Info().fX,pTile->Get_Info().fX });
        }
    }
}

void CRim::Check_LoggingWork()
{
    //�Ĺ��� �����ڿ� ��ü�� ������ �ִ��� Ȯ��         //�׸��� �� ���󰡴� ���̾ƴϰ�, �۾��������� �ƴҶ� �� , ���ο� �۾��� ������ �� �˻�
    if (!CColonyMgr::Get_Instance()->Get_LoggingSet()->empty() && Get_State() == WANDERING) //�̰� ���� ���μ������� �ſ� ���� �ϸ� �ɵ�?
    {
        //��ü�� ���� �� ���� ã�� �� �ִ� ���� ������
        //�ش� ���� �ֺ��� 8���� Ÿ���� Ȯ���ؼ� ���� ã�� �� �ִ��� Ȯ��
        //���� ��ã���� ��ü��������, ���� ã���� ��ü�Ϸ�����

        //Set�� vector�� ������ ����
        set<TASK>& LoggingSet = *CColonyMgr::Get_Instance()->Get_LoggingSet();
        vector<TASK> vecLogging(LoggingSet.begin(), LoggingSet.end());
        // ����� ���� ����: ���������� �Ÿ��� ����� ����
        std::sort(vecLogging.begin(), vecLogging.end(),
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
        for (TASK _tTask : vecLogging)
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
            for (auto Iter = LoggingSet.begin(); Iter != LoggingSet.end();)
            {
                if ((*Iter).pObj == m_pTarget)
                {
                    //���� ��
                    Iter = LoggingSet.erase(Iter);
                    //�����ؼ� �߰�
                    TASK tTask;
                    tTask.pObj = m_pTarget;
                    tTask.pRimReserved = this;
                    LoggingSet.emplace(tTask);
                }
                else
                {
                    ++Iter;
                }
            }

            m_bNavigating = true;
            Change_State(MOVETOWORK, _tTask.pObj);
            //Set_Target(_tTask.pObj);
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
            continue;
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

CObj* CRim::Find_Item(const TCHAR* _pImgKey)
{
    CObj* pClosestItem = nullptr;
    float fMinDistance = FLT_MAX; // �ʱ� �ּ� �Ÿ� ���� �ſ� ū ������ ����

    // ������ ����Ʈ���� ���� ����� �������� ã�´�.
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ITEM])
    {
        // �̹��� Ű�� ��ġ���� ������ �ǳʶ�
        if (lstrcmp(pObj->Get_ImgKey(), _pImgKey))
        {
            continue;
        }

        // ������ �ִ� �������� �ǳʶ�
        if (pObj->Get_Target())
        {
            continue;
        }

        // ���� ��ġ�� ������ ��ġ�� ������
        POS tStart{ (int)m_tInfo.fX, (int)m_tInfo.fY };
        POS tEnd{ (int)pObj->Get_Info().fX, (int)pObj->Get_Info().fY };

        // ��θ� ã��
        list<CNode*> PathList = move(CPathFinder::Get_Instance()->Find_Path(tStart, tEnd));

        // ��ΰ� ������ �ǳʶ�
        if (PathList.empty())
        {
            continue;
        }

        // ��带 ��� ����
        for_each(PathList.begin(), PathList.end(), Safe_Delete<CNode*>);
        PathList.clear();

        // �Ÿ� ��� (��Ÿ��� ����)
        float fDistance = sqrtf(powf(float(tEnd.iX - tStart.iX), 2.f) + powf(float(tEnd.iY - tStart.iY), 2.f));

        // ���� ����� �������� ����
        if (fDistance < fMinDistance)
        {
            fMinDistance = fDistance;
            pClosestItem = pObj;
        }
    }

    return pClosestItem;

}

void CRim::PickUp_Item(CObj* _pObj)
{
    //�ش�Ÿ�Ͽ� �������� �������ٰ� �˸�
    //POS tPos{ (int)m_pTarget->Get_Info().fX, (int)m_pTarget->Get_Info().fY };
    //CTileMgr::Get_Instance()->Set_TileObj(tPos, nullptr);

    m_pTransportingItem = _pObj;
    _pObj->Set_Target(this);
    _pObj = nullptr;
}

void CRim::PutDown_Item()
{
    if(!m_pTransportingItem)
    {
        return;
    }
    else
    {
        m_pTransportingItem->Set_Target(nullptr);
        m_pTransportingItem = nullptr;
    }

}

