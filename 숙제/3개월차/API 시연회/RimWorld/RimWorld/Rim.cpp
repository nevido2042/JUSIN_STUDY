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

CRim::CRim()
    :m_eState(END)
{

}

CRim::~CRim()
{
    Release();
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

    m_eState = DRAFTED; //현재는 소집됬지만 추후 기본은 자유행동으로
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

    if (m_bNavigating)
    {
        Navigate();
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
    //타겟과의 거리 및 각도 계산
    Measure_Target();

    //사정 거리내에 있고 본인이 죽지 않았다면
    if (IsWithinRange())
    {
        m_bAttack = true;
    }
    else
    {
        m_bAttack = false;
    }

    //마우스 클릭 했을 때 타겟으로 설정
    POINT	ptMouse{};

    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    ptMouse.x -= iScrollX;
    ptMouse.y -= iScrollY;

    if (PtInRect(&m_tRect, ptMouse))
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

    //식민지 관리자에 해체할 벽들이 있는지 확인         //그리고 길 따라가는 중이아니고, 작업상태중이 아닐때 만
    if (!CColonyMgr::Get_Instance()->Get_DeconstructSet()->empty()&& m_eState != WORKING) //이거 몬스터 벽부수러가는 거에 적용 하면 될듯?
    {
        //해체할 벽들 중 길을 찾을 수 있는 것이 나오면
        //해당 벽돌 주변의 8개의 타일을 확인해서 길을 찾을 수 있는지 확인
        //길을 못찾으면 해체하지말고, 길을 찾으면 해체하러가라

        for (CObj* pWall : *CColonyMgr::Get_Instance()->Get_DeconstructSet())
        {

            //올라갈 수 있는 타일이 있는가?
            CObj* pTile = CTileMgr::Get_Instance()->Find_ReachableTiles(pWall->Get_Info().fX, pWall->Get_Info().fY);
            if (!pTile)
            {
                continue;
            }
            //그 타일의 길을 찾을 수 있는가?
            list<CNode*> NodeList = CPathFinder::Get_Instance()
                ->Find_Path(POS{ m_tInfo.fX,m_tInfo.fY }, POS{ pTile->Get_Info().fX, pTile->Get_Info().fY });

            if (NodeList.empty())
            {
                continue;
            }

            Set_Target(pWall);

            m_NodeList = move(NodeList);
            m_bNavigating = true;
            m_eState = WORKING;
            //Move_To(POS{ pTile->Get_Info().fX,pTile->Get_Info().fX });
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


}

