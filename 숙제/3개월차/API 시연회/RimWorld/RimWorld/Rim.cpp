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

    //무기 생성
    m_pRangedWeapon = CAbstractFactory<CBoltActionRifle>::Create(m_tInfo.fX, m_tInfo.fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WEAPON, m_pRangedWeapon);
    m_pRangedWeapon->Set_Target(this);

    Take_Damage(10.f);

    m_eState = UNDRAFTED; //현재는 소집됬지만 추후 기본은 자유행동으로
}

int CRim::Update()
{
    if (m_bDestroyed)
        return OBJ_DESTROYED;

    //죽었으면 리턴
    if (m_bDead)
    {
        return OBJ_NOEVENT;
    }

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CRim::Late_Update()
{
    //죽었으면 리턴
    if (m_bDead)
    {
        return;
    }

    //이동방향 계산
    Calculate_MoveDir();

    if (m_pTarget)
    {
        //타겟과의 거리 및 각도 계산
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

    //마우스가 올라가져있는가?
    if (Is_MouseHovered())
    {
        //좌클릭은 타겟으로 설정
        if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
        {
            CColonyMgr::Get_Instance()->Set_Target(this);
            return;
        }
        //우클릭은 타겟의 공격 타겟으로 설정
        else if (CKeyMgr::Get_Instance()->Key_Up(VK_RBUTTON))
        {
            if (CObj* pTarget = CColonyMgr::Get_Instance()->Get_Target())
            {
                //자기 자신이 타겟이 될 수 없음.
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
        Ellipse(hDC, int(pNode->Get_Pos().fX + iScrollX - 10.f), int(pNode->Get_Pos().fY + iScrollY - 10.f),
            int(pNode->Get_Pos().fX + 10.f + iScrollX), int(pNode->Get_Pos().fY + 10.f + iScrollY));
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

}

void CRim::Deconstruct()
{
    m_pTarget->Set_Destroyed();
    m_eState = UNDRAFTED;
}

void CRim::Drafed()
{
    //사정 거리내에 있고 본인이 죽지 않았다면
    if (IsWithinRange())
    {
        //RequestNavStop();
        m_bAttack = true;
    }
    else
    {
        m_bAttack = false;
    }

    //타겟 있으면 따라가기
    if (m_pTarget)
    {
        //타겟이 죽으면 타겟 없애기
        CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
        if (pTarget->Get_IsDead())
        {
            Set_Target(nullptr);
        }
        else
        {
            //멈춰서 공격 중일 때 못찾게해야함!!!!!!!!!!!
            Move_To(POS{ m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY });
        }
    }

}

void CRim::Undrafed()
{
    //새로운 작업이 추가 됬다면
    if (CColonyMgr::Get_Instance()->Get_NewTaskAdded())
    {
        //작업을 찾아라
        m_bFindingTask = true;
    }

    //식민지 관리자에 해체할 벽들이 있는지 확인         //그리고 길 따라가는 중이아니고, 작업상태중이 아닐때 만 , 새로운 작업이 생겼을 때 검사
    if (!CColonyMgr::Get_Instance()->Get_DeconstructSet()->empty() && m_eState == UNDRAFTED && m_bFindingTask) //이거 몬스터 벽부수러가는 거에 적용 하면 될듯?
    {
        //해체할 벽들 중 길을 찾을 수 있는 것이 나오면
        //해당 벽돌 주변의 8개의 타일을 확인해서 길을 찾을 수 있는지 확인
        //길을 못찾으면 해체하지말고, 길을 찾으면 해체하러가라


        //Set을 vector로 복사후 정렬
        set<CObj*>& DeconstructSet = *CColonyMgr::Get_Instance()->Get_DeconstructSet();

        vector<CObj*> vecDeconstruct(DeconstructSet.begin(), DeconstructSet.end());


        // 사용자 정의 정렬: 기준점과의 거리를 계산해 정렬
        std::sort(vecDeconstruct.begin(), vecDeconstruct.end(), [this](CObj* _Dst, CObj* _Src) {
            float fDistA = CObj::Calculate_Dist(this, _Dst);
            float fDistB = CObj::Calculate_Dist(this, _Src);
            return fDistA < fDistB; // 거리가 가까울수록 앞쪽으로 정렬
            });

        //작업해야할 벽들 탐색(나에게 가장 가까운 녀석들로 정렬)
        for (CObj* pWall : vecDeconstruct)
        {
            //이동 가능한 타일이 있으면 노드리스트 반환
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

        //작업 할수 있는 벽을 못찾음.
        m_bFindingTask = false;

    }
}

void CRim::Work()
{
    //타겟이 가까운지 확인
    if (m_fTargetDist < TILECX * 1.5f)
    {
        RequestNavStop();
        CSteelWall* pWall = static_cast<CSteelWall*>(m_pTarget);
        pWall->Set_IsBrokenDown();
    }
    //타겟이 가까우면 해체 시작
    if (!m_bNavigating)
    {
        Deconstruct();
    }
    //해체 진행중인 바 생성
    //몇초 뒤 해체 완료
}

