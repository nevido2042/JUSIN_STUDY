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

    //무기 생성
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

    //마우스가 올라가져있는가?
    if (Is_MouseHovered_Scrolled())
    {
        //좌클릭은 타겟으로 설정
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

    // 림 몸통, 얼굴, 머리 순
    HDC hTestDC(nullptr);
    switch (m_eDir)
    {
    case UU:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_north");
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //얼굴
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_north");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // 머리
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
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //얼굴
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_east");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // 머리
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
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //얼굴
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_south");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // 머리
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
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //얼굴
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_west");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // 머리
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

    //죽었을 때 핏자국 출력
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
    

    //길 찾기 노드 출력
    for (CNode* pNode : m_NodeList)
    {
        Ellipse(hDC, int(pNode->Get_Pos().iX + iScrollX - 10.f), int(pNode->Get_Pos().iY + iScrollY - 10.f),
            int(pNode->Get_Pos().iX + 10.f + iScrollX), int(pNode->Get_Pos().iY + 10.f + iScrollY));
    }

    // 변수 값을 유니코드 문자열로 변환
    wchar_t buffer[50];
    wsprintf(buffer, L"m_bAttack: %d", m_bAttack);
    // 문자열 출력 (유니코드)
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY), buffer, lstrlenW(buffer));
    // 변수 값을 유니코드 문자열로 변환
    wsprintf(buffer, L"m_pTarget: %p", m_pTarget);
    // 문자열 출력 (유니코드)
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
    case CRim::TRANSPORTING:
        wsprintf(buffer, L"m_eState: %s", L"TRANSPORTING");
        break;
    case CRim::END:
        break;
    default:
        break;
    }
    // 문자열 출력 (유니코드)
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY + 40), buffer, lstrlenW(buffer));

}

void CRim::Handle_Wandering()
{
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
        Check_ConstructWork();
        Check_DeconstructWork();
        Check_TransportingWork();
        //m_fElapsedTimeCheck = 0.f;
        m_bTaskCheck = false;
    }
}

void CRim::Handle_Drafted()
{
    if (m_pTarget)
    {
        //타겟이 죽으면 타겟 없애기
        CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
        if (pTarget->Get_IsDead())
        {
            Set_Target(nullptr);
            //적이 죽었으면 다른 적을 찾는다.
            //Find_Enemy();
        }
        else
        {
            //멈춰서 공격 중일 때 못찾게해야함!!!!!!!!!!!
            //Move_To(POS{ m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY });
        }
    }
    else
    {
        Find_Enemy();
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
    }

}

void CRim::Handle_Undrafted()
{

}

void CRim::Handle_Deconstructing()
{
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
            Change_State(WANDERING);
            return;
        }
        //해체 하는거
        Deconstruct();
    }

    //해체 진행중인 바 생성
    //몇초 뒤 해체 완료
}

void CRim::Handle_Constructing()
{
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
    //타겟 아이템이 가까워지면 아이템을 들어라
    if (!m_pTransportingItem && m_fTargetDist < TILECX * 0.5f)
    {
        PickUp_Item();
        m_bNavigating = false;
    }
    //아이템을 들었으면 목표지점으로 옮겨라
    else if (m_pTransportingItem && !m_pTarget)
    {
        //옮길 지점을 찾아서 이동
        
        for (TASK tTask : *CColonyMgr::Get_Instance()->Get_TransportSet())
        {
            POS tStart{ (int)m_tInfo.fX,(int)m_tInfo.fY };
            POS tEnd{ (int)tTask.pObj->Get_Info().fX,(int)tTask.pObj->Get_Info().fY };

            list<CNode*> PathList = move(CPathFinder::Get_Instance()->Find_Path(tStart, tEnd));
            if (PathList.empty())
            {
                continue;
            }
            else
            {
                for_each(PathList.begin(), PathList.end(), Safe_Delete<CNode*>);
                PathList.clear();

                Set_Target(tTask.pObj);
                break;
            }
        }
        //옮기 지점을 못찾으면 Wander로 변경
        if (!m_pTarget)
        {
            PutDown_Item();
            Change_State(WANDERING);
        }
    }
    //목표지점도 찾았고, 운반할 아이템이 있으면 이동하라
    else if (m_pTransportingItem && m_pTarget && !m_bNavigating)
    {
        POS tPos{ (int)m_pTarget->Get_Info().fX, (int)m_pTarget->Get_Info().fY };
        Move_To(tPos);
    }
    //목표지점으로 운반하고 있는 중에 거리체크
    else if (m_pTransportingItem && m_pTarget && m_bNavigating)
    {
        if (m_fTargetDist < TILECX * 0.5f)
        {
            //타일 위에 철이 있다고 알리고
            POS tPos{ (int)m_pTarget->Get_Info().fX, (int)m_pTarget->Get_Info().fY };
            CTileMgr::Get_Instance()->Set_TileObj(tPos, m_pTransportingItem);
            //건설작업을 확인한다.
            Check_ConstructWork();

            //타일 위에 아이템을 올려둔다.
            PutDown_Item();
            
            //운반 작업목록에서 삭제한다.
            set<TASK>& TransportSet = *CColonyMgr::Get_Instance()->Get_TransportSet();
            
            for (auto iter = TransportSet.begin(); iter != TransportSet.end();)
            {
                if ((*iter).pObj == m_pTarget)
                {
                    iter = TransportSet.erase(iter);
                }
                else
                {
                    ++iter;
                }
            }

        }
    }
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

    //작업삭제
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
            Change_State(DECONSTRUCTING);
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

        //해당 타일 위에 철 아이템이 없으면 못지음.
        vector<TASK> vecConstruct;// (ConstructSet.begin(), ConstructSet.end());

        for (auto iter = ConstructSet.begin(); iter != ConstructSet.end(); ++iter)
        {
            //철이 있는 타일만 벡터로 집어넣음
            CTile* pTile = static_cast<CTile*>((*iter).pObj);
            //위에 아무것도 없으면 컨티뉴
            CObj* pObj = pTile->Get_Obj();
            if (!pObj)
            {
                continue;
            }
            //철 이면 작업 벡터로 넣기
            if (pObj->Get_ImgKey() == L"Steel_b")
            {
                vecConstruct.push_back(*iter);
            }
        }


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
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();

            //이동 가능한 타일이 있으면 노드리스트 반환
            m_NodeList = move(CTileMgr::Get_Instance()
                ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)tTask.pObj->Get_Info().fX, (int)tTask.pObj->Get_Info().fY }));

            if (m_NodeList.empty())
            {
                continue;
            }

            Set_Target(tTask.pObj);
            //작업 리스트에서 고른 작업을 예약했음을 표시
            for (auto Iter = ConstructSet.begin(); Iter != ConstructSet.end();)
            {
                if ((*Iter).pObj == m_pTarget)
                {
                    //삭제 후
                    Iter = ConstructSet.erase(Iter);
                    //수정해서 추가
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

void CRim::Check_TransportingWork()
{
    //운반 할 것이 있으면
    if (!CColonyMgr::Get_Instance()->Get_TransportSet()->empty() && Get_State() == WANDERING)
    {
        //철 아이템을 찾아서 들어라
        CObj* pItem = Find_Item(L"Steel_b");
        if (!pItem)
        {
            return;
        }
        else
        {
            POS tItemPos{ (int)pItem->Get_Info().fX, (int)pItem->Get_Info().fY };
            Move_To(tItemPos);
            Set_Target(pItem);
            Change_State(TRANSPORTING);
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

CObj* CRim::Find_Item(const TCHAR* _pImgKey)
{
    CObj* pItem(nullptr);
    //아이템 리스트에서 찾고자 하는 아이템을 찾아라
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ITEM])
    {
        if (lstrcmp(pObj->Get_ImgKey(), _pImgKey))
        {
            continue;
        }
        //찾고 그 아이템까지 도달 할 수 있는 길을 찾아라.
        POS tStart{ (int)m_tInfo.fX, (int)m_tInfo.fY };
        POS tEnd{ (int)pObj->Get_Info().fX, (int)pObj->Get_Info().fY };

        list<CNode*> PathList = move(CPathFinder::Get_Instance()->Find_Path(tStart, tEnd));
        if (PathList.empty())
        {
            //못 찾으면 컨티뉴
            continue;
        }
        else
        {
            //노드 딜리트
            for_each(PathList.begin(), PathList.end(), Safe_Delete<CNode*>);
            PathList.clear();

            //길을 찾았으면 그 아이템 리턴
            pItem = pObj;
            break;
        }
    }

    return pItem;
}

void CRim::PickUp_Item()
{
    m_pTransportingItem = m_pTarget;
    m_pTarget->Set_Target(this);
    m_pTarget = nullptr;
}

void CRim::PutDown_Item()
{
    if (m_pTarget)
    {
        m_pTransportingItem->Set_Pos(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
    }

    m_pTransportingItem->Set_Target(nullptr);
    m_pTransportingItem = nullptr;
}

