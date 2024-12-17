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

    //무기 생성
    m_pRangedWeapon = CAbstractFactory<CChargeBlasterLight>::Create(m_tInfo.fX, m_tInfo.fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WEAPON, m_pRangedWeapon);
    m_pRangedWeapon->Set_Target(this);

    Take_Damage(50.f);
}

int CCentipede::Update()
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
        POS tMoveToPos{ m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY };

        //타겟이 Pawn이라면
        if (CPawn* pPawnTarget = dynamic_cast<CPawn*>(m_pTarget))//타겟이 Pawn이고 죽었으면
        {
            //Pawn이 죽었다면
            if (pPawnTarget->Get_IsDead())
            {
                Set_Target(nullptr);
                return OBJ_NOEVENT;
            }
       
        }
        //타겟이 벽이면
        else if (CSteelWall* pWallTarget = dynamic_cast<CSteelWall*>(m_pTarget))
        {
            //부서졌다면
            if (pWallTarget->Get_IsBrokenDown())
            {
                //타겟 해제
                Set_Target(nullptr);
                return OBJ_NOEVENT;
            }

            //만약에 타겟이 벽이라면 벽 근처의 올라갈 수 있는 타일을 선택하게해야함!!!!!!! 길을 못찾음!!!!
            //이렇게는 잘안되네, 브레즌햄으로 타겟과 이어버린다음...그중 갈수 있는 타일 선택하는게 맞을 듯
            int iIndex = CTileMgr::Get_TileIndex(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
            iIndex = Get_ReachableToTarget();

            tMoveToPos.fX = (iIndex % TILEX) * TILEX + TILECX * 0.5f;
            tMoveToPos.fY = (iIndex / TILEY) * TILEX + TILECY * 0.5f;
        }
        
        //만약에 타겟이 벽이라면 벽 근처의 올라갈 수 있는 타일을 선택하게해야함!!!!!!!
        //멈춰서 공격 중일 때 못찾게해야함!!!!!!!!!!!
        Move_To(tMoveToPos);
        
    }

    if (m_bNavigating)
    {
        Navigate();
    }


    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CCentipede::Late_Update()
{
    //죽었으면 리턴
    if (m_bDead)
    {
        return;
    }

    Calculate_MoveDir();
    Measure_Target();

    //사정 거리내에 있고 본인이 죽지 않았다면
    if (IsWithinRange()&&!m_bDead)
    {
        //RequestNavStop();
        m_bAttack = true;
    }
    else
    {
        m_bAttack = false;
    }

    //FindTarget();
    //타겟이 없을 경우
    //림 오브젝트리스트에서 가장 가까운 림을 찾고 타겟으로 설정
    if (!m_pTarget)
    {
        Find_Target();
    }

    //마우스 클릭 했을 때 타겟으로 설정
    if (Is_MouseHovered())
    {
        //우클릭은 타겟의 공격 타겟으로 설정
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

    // 림 몸통, 얼굴, 머리 순
    HDC hTestDC(nullptr);
    switch (m_eDir)
    {
    case UU:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_north");
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_east");
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_south");
        // 몸통
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_WHITE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Centipede_west");
        // 몸통
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

    //길 찾기 노드 출력
    for (CNode* pNode : m_NodeList)
    {
        Ellipse(hDC, int(pNode->Get_Pos().fX + iScrollX - 10.f), int(pNode->Get_Pos().fY + iScrollY - 10.f),
            int(pNode->Get_Pos().fX + 10.f + iScrollX), int(pNode->Get_Pos().fY + 10.f + iScrollY));
    }

    //타겟 출력
    if (m_pTarget)
    {
        // 빨간색 펜 생성
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // 굵기 1, 빨간색
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);       // 기존 펜 저장 및 빨간색 펜 설정

        int iX = (int)m_pTarget->Get_Info().fX;
        int iY = (int)m_pTarget->Get_Info().fY;

        Ellipse(hDC, int(iX + iScrollX - 10), int(iY + iScrollY - 10),
            int(iX + 10 + iScrollX), int(iY + 10 + iScrollY));

        // 펜 정리
    	SelectObject(hDC, hOldBrush); // 기존 펜 복원
    	DeleteObject(hBrush);         // 빨간색 펜 삭제
    }

    // 변수 값을 유니코드 문자열로 변환
    wchar_t buffer[50];
    wsprintf(buffer, L"m_bAttack: %d", m_bAttack);
    // 문자열 출력 (유니코드)
    TextOutW(hDC, int(m_tInfo.fX+ iScrollX), int(m_tInfo.fY + iScrollY), buffer, lstrlenW(buffer));
    // 변수 값을 유니코드 문자열로 변환
    wsprintf(buffer, L"m_pTarget: %p", m_pTarget);
    // 문자열 출력 (유니코드)
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY + 20), buffer, lstrlenW(buffer));
}

void CCentipede::Find_Target()
{
    list<CObj*> RimList = CObjMgr::Get_Instance()->Get_List()[OBJ_RIM];
    float fMinDist(0.f);
    CObj* pTarget(nullptr);

    for (CObj* pObj : RimList)
    {
        //죽은 놈이면 컨티뉴
        CPawn* pPawn = static_cast<CPawn*>(pObj);
        if (pPawn->Get_IsDead())
        {
            continue;
        }

        float fWidth(0.f);
        float fHeight(0.f);
        float fDist(0.f);
        //거리 계산
        fWidth = pObj->Get_Info().fX - Get_Info().fX;
        fHeight = pObj->Get_Info().fY - Get_Info().fY;
        fDist = sqrtf(fWidth * fWidth + fHeight * fHeight);
        //타겟이 없거나, 거리가 더 가까운 애를 발견하면 타겟 변경
        if (!pTarget||fDist < fMinDist)
        {
            pTarget = pObj;
            fMinDist = fDist;
        }
    }

    Set_Target(pTarget);
}
