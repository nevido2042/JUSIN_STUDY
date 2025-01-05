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

CRim::CRim()
    :m_bTaskCheck(false), m_pTransportingItem(nullptr)
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

    CPawn::Render(hDC);

    /*Ellipse(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
        m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);*/

    // �� ����, ��, �Ӹ� ��
    HDC hTestDC(nullptr);
    switch (m_eDir)
    {
    case UU:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Body[NORTH].c_str());
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //��
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Face[NORTH].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // �Ӹ�
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Hair[NORTH].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Body[EAST].c_str());
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //��
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Face[EAST].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // �Ӹ�
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Hair[EAST].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Body[SOUTH].c_str());
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //��
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Face[SOUTH].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // �Ӹ�
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Hair[SOUTH].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Body[WEST].c_str());
        // ����
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //��
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Face[WEST].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // �Ӹ�
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Hair[WEST].c_str());
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
        Check_LoggingWork();
        //Check_TransportingWork();
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
        if (!m_bNavigating)
        {
            Find_Enemy();
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
        Set_Target(nullptr);
        m_bAttack = false;
    }

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
    if (m_fTargetDist < TILECX * 1.2f)
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
        if (m_fTargetDist > TILECX * 1.2f)
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

    //ö�� ��ã������
    if (!m_pTarget)
    {
        CObj* pItem = Find_Item(L"Steel_b");

        if (pItem)
        {
            //�� �� �ִ� ö�� ������
            //������� ����
            m_pTarget = pItem;

            POS tPos{ (int)pItem->Get_Info().fX, (int)pItem->Get_Info().fY };
            Move_To(tPos);
            return;
            //PickUp_Item(pItem);
        }
        else
        {
            m_bTaskCheck = true;
            Change_State(WANDERING);
            PutDown_Item();
            return;
        }
    }

    //Ÿ�� �ְ�, ����ϴ°� ����, �׺���̼� �������� ����
    if (m_pTarget && !m_pTransportingItem && !m_bNavigating)
    {
        m_bTaskCheck = true;
        Change_State(WANDERING);
        PutDown_Item();
        return;
    }


    //����ϰ��ִ°� ������
    if (m_pTransportingItem)
    {
        m_pTarget = nullptr;

        Check_ConstructWork();

        //�Ǽ� �Ұ� ������
        if (Get_State() != CONSTRUCTING)
        {
            m_bTaskCheck = true;
            Change_State(WANDERING);
            PutDown_Item();
        }
    }

    //Ÿ�� �������� ��������� �������� ����
    if (!m_pTransportingItem && m_fTargetDist < TILECX * 0.5f)
    {
        if (m_pTarget)
        {
            PickUp_Item(m_pTarget);
        }
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

    //Ÿ���� ������� Ȯ��
    if (m_fTargetDist < TILECX * 1.2f)
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


    }
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

    //��ü �ð� �ɸ��� �ϰ� ���� ���
    if (m_fConstructTime > m_fConstructElapsed)
    {
        //�帱�Ҹ� ����
        CSoundMgr::Get_Instance()->PlaySound(L"DrillB.wav", SOUND_CONSTRUCT, .5f);
        m_fConstructElapsed += GAMESPEED;
        return;
    }

    m_fConstructElapsed = 0.f;

    CSoundMgr::Get_Instance()->StopSound(SOUND_CONSTRUCT);
    CSoundMgr::Get_Instance()->PlaySound(L"HammerA.wav", SOUND_CONSTRUCT, .5f);

    //��� �ִ� ö�� ����
    m_pTransportingItem->Set_Destroyed();
    m_pTransportingItem = nullptr;

    if (m_eCurrentTask.eType == TASK::SHIP)
    {
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
        //�� �Ǽ�
        CObj* pObj = CAbstractFactory<CSteelWall>::Create(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
        CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
        CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, pObj);
    }

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
            Change_State(DECONSTRUCTING, _tTask.pObj);
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

        //���� ö�� ��� �־�߸� �Ǽ� ����
        //ö�� �ȵ������ ö ã�Ƽ� ����
        //ö�� �־����
        if (!m_pTransportingItem)
        {
            CObj* pItem = Find_Item(L"Steel_b");

            if (pItem)
            {
                Change_State(TRANSPORTING);
                return;
            }
            else
            {
                return;
            }
        }

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
            Change_State(CONSTRUCTING, tTask.pObj);
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
    CObj* pItem(nullptr);
    //������ ����Ʈ���� ã���� �ϴ� �������� ã�ƶ�
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ITEM])
    {
        if (lstrcmp(pObj->Get_ImgKey(), _pImgKey))
        {
            continue;
        }

        //������ ������
        if (pObj->Get_Target())
        {
            continue;
        }

        //ã�� �� �����۱��� ���� �� �� �ִ� ���� ã�ƶ�.
        POS tStart{ (int)m_tInfo.fX, (int)m_tInfo.fY };
        POS tEnd{ (int)pObj->Get_Info().fX, (int)pObj->Get_Info().fY };

        list<CNode*> PathList = move(CPathFinder::Get_Instance()->Find_Path(tStart, tEnd));
        if (PathList.empty())
        {
            //�� ã���� ��Ƽ��
            continue;
        }
        else
        {
            //��� ����Ʈ
            for_each(PathList.begin(), PathList.end(), Safe_Delete<CNode*>);
            PathList.clear();

            //���� ã������ �� ������ ����
            pItem = pObj;
            break;
        }
    }

    return pItem;
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

