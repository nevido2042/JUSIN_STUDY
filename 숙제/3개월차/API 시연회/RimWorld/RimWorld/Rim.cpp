#include "pch.h"
#include "Rim.h"
#include "BmpMgr.h"

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

    //무기 생성
    m_pRangedWeapon = CAbstractFactory<CBoltActionRifle>::Create(m_tInfo.fX, m_tInfo.fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WEAPON, m_pRangedWeapon);
    m_pRangedWeapon->Set_Target(this);

    //Take_Damage(10.f);

    Change_State(WANDERING);

    //캐릭터 이미지 설정
    //m_pImgKey_Body = L"Naked_Female"; //L"Naked_Male";
    //m_pImgKey_Face = L"Female_Average_Normal";//L"Male_Average_Normal";
    //m_pImgKey_Hair = L"Decent"; //L"Afro";

    int iRand(0);
    iRand = rand() % 3;
    // 랜덤으로 Body 이미지 선택
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

    // 랜덤으로 Face 이미지 선택
    if (rand() % 2 == 0) 
    {
        m_pImgKey_Face = L"Female_Average_Normal";
    }
    else 
    {
        m_pImgKey_Face = L"Male_Average_Normal";
    }

    // 랜덤으로 Hair 이미지 선택
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

    // 림 몸통, 얼굴, 머리 순
    HDC hTestDC(nullptr);


    switch (m_eDir) 
    {
    case UU:
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Body[NORTH], IMAGE_OFFSET_X, IMAGE_OFFSET_Y); // 몸통
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Face[NORTH], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // 얼굴
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Hair[NORTH], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // 머리
        break;

    case RR:
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Body[EAST], IMAGE_OFFSET_X, IMAGE_OFFSET_Y); // 몸통
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Face[EAST], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // 얼굴
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Hair[EAST], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // 머리
        break;

    case DD:
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Body[SOUTH], IMAGE_OFFSET_X, IMAGE_OFFSET_Y); // 몸통
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Face[SOUTH], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // 얼굴
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Hair[SOUTH], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // 머리
        break;

    case LL:
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Body[WEST], IMAGE_OFFSET_X, IMAGE_OFFSET_Y); // 몸통
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Face[WEST], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // 얼굴
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Hair[WEST], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // 머리
        break;

    default:
        break;
    }


    //죽었을 때 핏자국 출력
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
    //일정 시간마다 작업 체크 하는거
    //m_fTaskCheckTime_Elapsed += GAMESPEED;
    //if (m_fTaskCheckTime < m_fTaskCheckTime_Elapsed)
    //{
    //    //m_bTaskCheck = true;
    //    m_fTaskCheckTime_Elapsed = 0.f;
    //}

    //템 들고 있지 않도록
    if (m_pTransportingItem)
    {
        PutDown_Item();
    }

    //배회 하자
    Wander();

    //새로운 작업이 생겼다면?????????? 작업의 갯수가 달라졌다면?
    //작업 목록이 달라졌다면? 작업리스트를 림이 복사에서 가지고 있는다?
    // 
    //####콜로니 매니저에서 작업리스트 바뀌었다고 알려주면?<이게 맞는듯?>
    //####콜로니 매니저에서 작업이 바뀌었을 때(추가, 삭제) 모든 림에게, 작업 확인하라 지시
    
    //작업을 체크할 시간이 됬다면(이거 빼버리고)
    if (m_bTaskCheck)
    {
        //건설이 앞에 서면, 재료가 없어도 transport 상태가 되서 해체(?), 벌목을 안한다
        Check_ConstructWork();
        //순서가 중요함
        Check_DeconstructWork();
        Check_LoggingWork();

        //Check_TransportingWork();
        //m_fElapsedTimeCheck = 0.f;
        m_bTaskCheck = false;
    }
}

void CRim::Handle_Chasing()
{
    CPawn::Handle_Chasing();
}

void CRim::Handle_Drafted()
{
    //적 탐색 상태
    //적 조준 상태
    //적 사격 가능 상태

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
    //    //사정거리에 있고 적이 보인다면
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
    //    //타겟이 죽으면 타겟 없애기
    //    CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
    //    if (pTarget->Get_IsDead())
    //    {
    //        Set_Target(nullptr);
    //        //적이 죽었으면 다른 적을 찾는다.
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
    //건설 할때 이미 벽이 있으면 철 내려놓고 다른 일 찾아라
    if (CTileMgr::Get_Instance()->Get_TileOption(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY) == OPT_BLOCKED)
    {
        m_bTaskCheck = true;
        Change_State(WANDERING);
        PutDown_Item();
        return;
    }

    //옮기는 철이 없다.
    if (!m_pTransportingItem)
    {
        m_bTaskCheck = true;
        Change_State(WANDERING);
        return;
    }

    //타겟이 가까운지 확인
    if (m_fTargetDist < TILECX * 1.5f)
    {
        //가까우면 멈춘다.
        //RequestNavStop();
        m_bNavigating = false;
    }
    //멈췃는데
    if (!m_bNavigating)
    {
        //타겟이 없다.
        if (!m_pTarget)
        {
            return;
        }
        //타겟이 멀다
        if (m_fTargetDist > TILECX * 1.5f)
        {
            //다른 작업 체크
            m_bTaskCheck = true;
            m_pTarget = nullptr;
            Change_State(WANDERING);
            return;
        }
        //건설
        Construct();
    }

    //해체 진행중인 바 생성
    //몇초 뒤 해체 완료
}

void CRim::Handle_Transporting()
{
    //처음들어올때 타겟이 없는게 정상
    //타겟 아이템이 가까워지면 아이템을 들어라
    if (m_pTarget)
    {
        if (!m_pTransportingItem && m_fTargetDist < TILECX * 1.5f)
        {

            PickUp_Item(m_pTarget);

        }
    }

    //현재 건설작업이 뭔지 파악하고, 어떤 재료를 가져가야할지 판단
    if (!m_pTarget)
    {
        CObj* pItem(nullptr);

        if (m_eCurrentTask.eType == TASK::CAMPFIRE)
        {
            //나무
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
            //철
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


    //타겟으로 한 철이 다른 애가 이미 가져갔으면
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

    //아이템을 들었으면 건설하러가라
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
    
    //타겟(우주선)과 거리가 가까워지면 탑승
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
            //기존 노드 딜리트
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();

            //이동 가능한 타일이 있으면 노드리스트 반환
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
                //다른 작업 체크
                m_bTaskCheck = true;
                Change_State(WANDERING);
                return;
            }
        }
        //타겟이 파괴됬다.
        if (m_pTarget->Get_Destroyed())
        {
            //다른 작업 체크
            m_bTaskCheck = true;
            Change_State(WANDERING);
            return;
        }
    }
    else
    {
        //다른 작업 체크
        m_bTaskCheck = true;
        Change_State(WANDERING);
        return;
    }

    //타겟이 가까운지 확인
    if (m_fTargetDist < TILECX * 1.5f)
    {
        //가까우면 멈춘다.
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
        //타겟이 죽으면 타겟 없애기
        CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
        if (pTarget->Get_IsDead())
        {
            Set_Target(nullptr);
            //적이 죽었으면 다른 적을 찾는다.
            Change_DraftedState(DRAFTED_SEARCH);
        }
    }


    if (m_pTarget)
    {
        //사정거리에 있고 적이 보인다면
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
        //타겟이 죽으면 타겟 없애기
        CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
        if (pTarget->Get_IsDead())
        {
            Set_Target(nullptr);
            //적이 죽었으면 다른 적을 찾는다.
            m_bAttack = false;
            Change_DraftedState(DRAFTED_SEARCH);
        }
    }

    //사정거리에 있고 적이 보인다면
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

    // 이미지 그리기 함수
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

    //해당 벽이 무너질 때 까지 공격 반복
    //시간 공격하는 시간 정해야함

    pWall->Take_Damage(1.f);

}

void CRim::Construct()
{
    //철 삭제
    //CObj* pSteel =  CTileMgr::Get_Instance()->Get_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
    //pSteel->Set_Destroyed();

    //이미 타일 위에 뭐가 있으면 취소 시키기
    if (CTileMgr::Get_Instance()->Get_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY))
    {
        m_bTaskCheck = true;
        PutDown_Item();
        Change_State(WANDERING);
        return;
    }

    //해체 시간 걸리게 하고 사운드 출력
    if (m_fConstructTime > m_fConstructElapsed)
    {
        switch (m_eCurrentTask.eType)
        {
        case TASK::TYPE::CAMPFIRE:
            //나무소리 나게
            //CSoundMgr::Get_Instance()->StopSound(SOUND_CONSTRUCT_WOOD);
            CSoundMgr::Get_Instance()->PlaySound(L"Hammer_Nail_Wood_1a.wav", SOUND_CONSTRUCT_WOOD, .5f);
            break;
        case TASK::TYPE::WALL:
        case TASK::TYPE::SHIP:
            //드릴소리 나게
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

    //들고 있는 철을 삭제
    m_pTransportingItem->Set_Destroyed();
    m_pTransportingItem = nullptr;

    if (m_eCurrentTask.eType == TASK::SHIP)
    {
        if (CTutorialMgr::Get_Instance()->Get_CurQuest() == CTutorialMgr::QUEST_CONSTRUCT_SHIP)
        {
            CTutorialMgr::Get_Instance()->IncreaseQuestProgress();
        }

        //우주선 건설
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

        //벽 건설
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

        //모닥불 건설
        CObj* pObj = CAbstractFactory<CCampfire>::Create(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
        CObjMgr::Get_Instance()->Add_Object(OBJ_CAMPFIRE, pObj);
        CTileMgr::Get_Instance()->Set_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, pObj);
    }

    //작업삭제
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

    //해당 벽이 무너질 때 까지 공격 반복
    //시간 공격하는 시간 정해야함

    pTree->Take_Damage(1.f);

    if (pTree->Get_IsBrokenDown())
    {
        m_pTarget = nullptr;
    }

}

void CRim::Check_DeconstructWork()
{
    //식민지 관리자에 해체할 벽들이 있는지 확인         //그리고 길 따라가는 중이아니고, 작업상태중이 아닐때 만 , 새로운 작업이 생겼을 때 검사
    if (!CColonyMgr::Get_Instance()->Get_DeconstructSet()->empty() && Get_State() == WANDERING) //이거 몬스터 벽부수러가는 거에 적용 하면 될듯?
    {
        //해체할 벽들 중 길을 찾을 수 있는 것이 나오면
        //해당 벽돌 주변의 8개의 타일을 확인해서 길을 찾을 수 있는지 확인
        //길을 못찾으면 해체하지말고, 길을 찾으면 해체하러가라

        //Set을 vector로 복사후 정렬
        set<TASK>& DeconstructSet = *CColonyMgr::Get_Instance()->Get_DeconstructSet();
        vector<TASK> vecDeconstruct(DeconstructSet.begin(), DeconstructSet.end());
        // 사용자 정의 정렬: 기준점과의 거리를 계산해 정렬
        std::sort(vecDeconstruct.begin(), vecDeconstruct.end(), 
            [this](const TASK _tTaskA, const TASK _tTaskB) 
            {
                // 예약되지 않으면 우선순위 앞으로
                if (!_tTaskA.pRimReserved && _tTaskB.pRimReserved)
                {
                    return true;  // _tTaskA가 더 앞에 오도록
                }

                if (_tTaskA.pRimReserved && !_tTaskB.pRimReserved)
                {
                    return false; // _tTaskB가 더 앞에 오도록
                }

                float fDistA = CObj::Calculate_Dist(this, _tTaskA.pObj);
                float fDistB = CObj::Calculate_Dist(this, _tTaskB.pObj);
                return fDistA < fDistB; // 거리가 가까울수록 앞쪽으로 정렬
            });

        //작업해야할 벽들 탐색(나에게 가장 가까운 녀석들로 정렬)
        for (TASK _tTask : vecDeconstruct)
        {
            ////기존 노드 딜리트
            //for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            //m_NodeList.clear();

            ////이동 가능한 타일이 있으면 노드리스트 반환
            //m_NodeList = move(CTileMgr::Get_Instance()
            //    ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)_tTask.pObj->Get_Info().fX, (int)_tTask.pObj->Get_Info().fY }));

            //if (m_NodeList.empty())
            //{
            //    continue;
            //}

            Set_Target(_tTask.pObj);
            //작업 리스트에서 고른 작업을 예약했음을 표시
            for (auto Iter = DeconstructSet.begin(); Iter != DeconstructSet.end();)
            {
                if ((*Iter).pObj == m_pTarget)
                {
                    //삭제 후
                    Iter = DeconstructSet.erase(Iter);
                    //수정해서 추가
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
    //식민지 관리자에 해체할 벽들이 있는지 확인         //그리고 길 따라가는 중이아니고, 작업상태중이 아닐때 만 , 새로운 작업이 생겼을 때 검사
    if (!CColonyMgr::Get_Instance()->Get_ConstructSet()->empty() && (Get_State() == WANDERING|| Get_State() == TRANSPORTING)) //이거 몬스터 벽부수러가는 거에 적용 하면 될듯?
    {


        //해체할 벽들 중 길을 찾을 수 있는 것이 나오면
        //해당 벽돌 주변의 8개의 타일을 확인해서 길을 찾을 수 있는지 확인
        //길을 못찾으면 해체하지말고, 길을 찾으면 해체하러가라

        //Set을 vector로 복사후 정렬
        set<TASK>& ConstructSet = *CColonyMgr::Get_Instance()->Get_ConstructSet();
        vector<TASK> vecConstruct(ConstructSet.begin(), ConstructSet.end());


        // 사용자 정의 정렬: 기준점과의 거리를 계산해 정렬
        std::sort(vecConstruct.begin(), vecConstruct.end(), 
            [this](const TASK _tTaskA, const TASK _tTaskB)
            {
                // 예약되지 않으면 우선순위 앞으로
                if (!_tTaskA.pRimReserved && _tTaskB.pRimReserved)
                {
                    return true;  // _tTaskA가 더 앞에 오도록
                }

                if (_tTaskA.pRimReserved && !_tTaskB.pRimReserved)
                {
                    return false; // _tTaskB가 더 앞에 오도록
                }

                float fDistA = CObj::Calculate_Dist(this, _tTaskA.pObj);
                float fDistB = CObj::Calculate_Dist(this, _tTaskB.pObj);
                return fDistA < fDistB; // 거리가 가까울수록 앞쪽으로 정렬
            });

        //작업해야할 벽들 탐색(나에게 가장 가까운 녀석들로 정렬)
        for (TASK tTask : vecConstruct)
        {
            //기존 노드 딜리트
            //for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            //m_NodeList.clear();

            //이동 가능한 타일이 있으면 노드리스트 반환
            list<CNode*> NodeList = move(CTileMgr::Get_Instance()
                ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)tTask.pObj->Get_Info().fX, (int)tTask.pObj->Get_Info().fY }));

            if (NodeList.empty())
            {
                continue;
            }

            for_each(NodeList.begin(), NodeList.end(), Safe_Delete<CNode*>);
            NodeList.clear();

            //재료가 있는지 확인
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
            //작업 리스트에서 고른 작업을 예약했음을 표시
            for (auto Iter = ConstructSet.begin(); Iter != ConstructSet.end();)
            {
                if ((*Iter).pObj == m_pTarget)
                {
                    TASK tTas = *Iter;
                    //삭제 후
                    Iter = ConstructSet.erase(Iter);
                    //수정해서 추가
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
    //식민지 관리자에 해체할 벽들이 있는지 확인         //그리고 길 따라가는 중이아니고, 작업상태중이 아닐때 만 , 새로운 작업이 생겼을 때 검사
    if (!CColonyMgr::Get_Instance()->Get_LoggingSet()->empty() && Get_State() == WANDERING) //이거 몬스터 벽부수러가는 거에 적용 하면 될듯?
    {
        //해체할 벽들 중 길을 찾을 수 있는 것이 나오면
        //해당 벽돌 주변의 8개의 타일을 확인해서 길을 찾을 수 있는지 확인
        //길을 못찾으면 해체하지말고, 길을 찾으면 해체하러가라

        //Set을 vector로 복사후 정렬
        set<TASK>& LoggingSet = *CColonyMgr::Get_Instance()->Get_LoggingSet();
        vector<TASK> vecLogging(LoggingSet.begin(), LoggingSet.end());
        // 사용자 정의 정렬: 기준점과의 거리를 계산해 정렬
        std::sort(vecLogging.begin(), vecLogging.end(),
            [this](const TASK _tTaskA, const TASK _tTaskB)
            {
                // 예약되지 않으면 우선순위 앞으로
                if (!_tTaskA.pRimReserved && _tTaskB.pRimReserved)
                {
                    return true;  // _tTaskA가 더 앞에 오도록
                }

                if (_tTaskA.pRimReserved && !_tTaskB.pRimReserved)
                {
                    return false; // _tTaskB가 더 앞에 오도록
                }

                float fDistA = CObj::Calculate_Dist(this, _tTaskA.pObj);
                float fDistB = CObj::Calculate_Dist(this, _tTaskB.pObj);
                return fDistA < fDistB; // 거리가 가까울수록 앞쪽으로 정렬
            });

        //작업해야할 벽들 탐색(나에게 가장 가까운 녀석들로 정렬)
        for (TASK _tTask : vecLogging)
        {
            //기존 노드 딜리트
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();

            //이동 가능한 타일이 있으면 노드리스트 반환
            m_NodeList = move(CTileMgr::Get_Instance()
                ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)_tTask.pObj->Get_Info().fX, (int)_tTask.pObj->Get_Info().fY }));

            if (m_NodeList.empty())
            {
                continue;
            }

            Set_Target(_tTask.pObj);
            //작업 리스트에서 고른 작업을 예약했음을 표시
            for (auto Iter = LoggingSet.begin(); Iter != LoggingSet.end();)
            {
                if ((*Iter).pObj == m_pTarget)
                {
                    //삭제 후
                    Iter = LoggingSet.erase(Iter);
                    //수정해서 추가
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
    float fMinDistance = FLT_MAX; // 초기 최소 거리 값을 매우 큰 값으로 설정

    // 아이템 리스트에서 가장 가까운 아이템을 찾는다.
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ITEM])
    {
        // 이미지 키가 일치하지 않으면 건너뜀
        if (lstrcmp(pObj->Get_ImgKey(), _pImgKey))
        {
            continue;
        }

        // 주인이 있는 아이템은 건너뜀
        if (pObj->Get_Target())
        {
            continue;
        }

        // 현재 위치와 아이템 위치를 가져옴
        POS tStart{ (int)m_tInfo.fX, (int)m_tInfo.fY };
        POS tEnd{ (int)pObj->Get_Info().fX, (int)pObj->Get_Info().fY };

        // 경로를 찾음
        list<CNode*> PathList = move(CPathFinder::Get_Instance()->Find_Path(tStart, tEnd));

        // 경로가 없으면 건너뜀
        if (PathList.empty())
        {
            continue;
        }

        // 노드를 모두 삭제
        for_each(PathList.begin(), PathList.end(), Safe_Delete<CNode*>);
        PathList.clear();

        // 거리 계산 (피타고라스 공식)
        float fDistance = sqrtf(powf(float(tEnd.iX - tStart.iX), 2.f) + powf(float(tEnd.iY - tStart.iY), 2.f));

        // 가장 가까운 아이템을 선택
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
    //해당타일에 아이템이 없어졌다고 알림
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

