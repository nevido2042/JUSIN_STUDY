#include "pch.h"
#include "ColonyMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "Rim.h"
#include "TimeMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"

CColonyMgr* CColonyMgr::m_pInstance = nullptr;

CColonyMgr::CColonyMgr()
	:m_pTarget(nullptr), m_eMode(MODE_END)
{
}

CColonyMgr::~CColonyMgr()
{
    Release();
}

void CColonyMgr::Change_Mode(MODE _eMode)
{
    switch (_eMode)
    {
    case CColonyMgr::MODE_SELECT:
        break;
    case CColonyMgr::MODE_DECONSTRUCT:
        //타겟을 없애고,
        //마우스에 해체 모양 아이콘을 띄운다.
        //마우스가 위치하는 타일에 강조표시를 띄운다.
        m_pTarget = nullptr;
        break;
    case CColonyMgr::MODE_CONSTRUCT:
        //타겟을 없애고,
        //마우스에 철벽 모양 아이콘을 띄운다.
        //마우스가 위치하는 타일에 강조표시를 띄운다.
        m_pTarget = nullptr;
        break;
    default:
        break;
    }

    m_eMode = _eMode;
}

void CColonyMgr::Emplace_DeconstructSet(CObj* _pObj)
{
    m_DeconstructSet.emplace(_pObj);

    Notify_TaskChange();
}

void CColonyMgr::Emplace_ConstructSet(CObj* _pObj)
{
    m_ConstructSet.emplace(_pObj);

    Notify_TaskChange();
}

void CColonyMgr::Notify_TaskChange()
{
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_RIM])
    {
        CRim* pRim = static_cast<CRim*>(pObj);
        pRim->Set_TaskCheck();
    }
}

void CColonyMgr::Input_Key()
{
    //화면 스크롤
    float fSpeed(10.f);
    if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
    {
        CScrollMgr::Get_Instance()->Move_ScrollX(fSpeed);
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
    {
        CScrollMgr::Get_Instance()->Move_ScrollX(-fSpeed);
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
    {
        CScrollMgr::Get_Instance()->Move_ScrollY(fSpeed);
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
    {
        CScrollMgr::Get_Instance()->Move_ScrollY(-fSpeed);
    }

    //게임 스피드 조절
    if (CKeyMgr::Get_Instance()->Key_Down('1'))
    {
        CTimeMgr::Get_Instance()->Set_GameSpeed(1.f);
    }
    else if (CKeyMgr::Get_Instance()->Key_Down('2'))
    {
        CTimeMgr::Get_Instance()->Set_GameSpeed(2.f);
    }
    else if (CKeyMgr::Get_Instance()->Key_Down('3'))
    {
        CTimeMgr::Get_Instance()->Set_GameSpeed(3.f);
    }
    else if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
    {
        if (GAMESPEED == 0.f)
        {
            CTimeMgr::Get_Instance()->Set_GameSpeed(1.f);
        }
        else
        {
            CTimeMgr::Get_Instance()->Set_GameSpeed(0.f);
        } 
    }
    
    //R 누를 시 림 소집/비소집 상태 변경
    if (CKeyMgr::Get_Instance()->Key_Down('R'))
    {
        if (m_pTarget)
        {
            if (CRim* pTargetRim = dynamic_cast<CRim*>(m_pTarget))
            {
                if (pTargetRim->Get_State() == CRim::DRAFTED)
                {
                    pTargetRim->Change_State(CRim::WANDERING);
                }
                else
                {
                    pTargetRim->Change_State(CRim::DRAFTED);
                }  
            }
        }
    }

    //esc 누를 시 선택모드로 돌아가기
    if (CKeyMgr::Get_Instance()->Key_Down(VK_ESCAPE))
    {
        Change_Mode(MODE_SELECT);
    }
}

void CColonyMgr::Initialize()
{
    m_eMode = MODE_SELECT;
}

int CColonyMgr::Update()
{
    Input_Key();

    return 0;
}

void CColonyMgr::Late_Update()
{
    CScrollMgr::Get_Instance()->Scroll_Lock();

    Control_Target();

    //이거 Late_Update 호출 순서에 따라 달라질수 있음(ObjMgr보다 늦게 호출되어야함)
    //새로운 작업이 들어왔는지 나타내는 불변수                     //개선 필요함 이상함 매우 불안한 코드
    //림이 주기적으로 할일 이 있는지 확인하게하는게 맞을듯?
}

void CColonyMgr::Render(HDC hDC)
{
    HDC		hDeconstructDC = CBmpMgr::Get_Instance()->Find_Image(L"Deconstruct_mini");
    HDC		hSteelWallDC = CBmpMgr::Get_Instance()->Find_Image(L"RockSmooth_MenuIcon_mini");

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    //해체 리스트에 있는 모든 벽들에 해체 아이콘 표시
    for (CObj* pObj : m_DeconstructSet)
    {
        GdiTransparentBlt(hDC,
            (int)pObj->Get_Info().fX + iScrollX - IMAGE_OFFSET_X,
            (int)pObj->Get_Info().fY + iScrollY - IMAGE_OFFSET_Y,
            64,
            64,
            hDeconstructDC,
            0, 0,
            64,
            64,
            RGB_WHITE);
    }
    //건설할 벽들 표시
    for (CObj* pObj : m_ConstructSet)
    {
        BitBlt(hDC,
            (int)pObj->Get_Info().fX + iScrollX - 8,
            (int)pObj->Get_Info().fY + iScrollY - 16,
            32,
            32,
            hSteelWallDC,
            0, 0,
            SRCCOPY);
    }

    //해체 모드일 경우 마우스에 해체 그림 표시
    if (m_eMode == MODE_DECONSTRUCT)
    {
        POINT	ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        GdiTransparentBlt(hDC,
            ptMouse.x,
            ptMouse.y,
            64,
            64,
            hDeconstructDC,
            0, 0,
            64,
            64,
            RGB_WHITE);
    }
    else if (m_eMode == MODE_CONSTRUCT)
    {
        POINT	ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        BitBlt(hDC,
            ptMouse.x,
            ptMouse.y,
            64,
            64,
            hSteelWallDC,
            0, 0,
            SRCCOPY);
    }
}

void CColonyMgr::Release()
{
    m_ConstructSet.clear();
}

void CColonyMgr::Control_Target()
{
    if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON) && m_pTarget)
    {
        int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
        int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

        //마우스 입력
        SetCapture(g_hWnd);
        POINT	ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        RECT tClientRect;
        GetClientRect(g_hWnd, &tClientRect);

        if (PtInRect(&tClientRect, ptMouse))
        {
            CRim* pTargetRim = static_cast<CRim*>(m_pTarget);
            //만약 Rim이 소집상태가 아니라면 임의적으로 움직이지 못하도록
            if (pTargetRim->Get_State() != CRim::DRAFTED)
            {
                return;
            }
            //마우스 좌표를 잘 깎아서 넣어야 한다.

            float fX = float(ptMouse.x - iScrollX);
            float fY = float(ptMouse.y - iScrollY);

            fX = float(fX - (int)fX % TILECX + TILECX * 0.5f);
            fY = float(fY - (int)fY % TILECY + TILECY * 0.5f);
            //Pawn을 클릭했으면 타겟으로 설정하고 공격명령 내려야함
            //타일을 클릭햇으면 이동시키고, 공격명령 취소, 타겟 제거


            //네비게이팅 중이었으면
            //if (pTargetRim->Get_IsNavigating())
            //{
            //    //네비게이팅 종료 요청을 보낸다.
            //    pTargetRim->RequestNavStop();
            //}

            pTargetRim->Move_To(POS{ (int)fX, (int)fY });
            //림이 공격 중이었고 타겠도 있었다면 해제
            if (pTargetRim->Get_Target() && pTargetRim->Get_IsAttack())
            {
                pTargetRim->Set_Target(nullptr);
                pTargetRim->Set_IsAttack(false);
            }
        }

        ReleaseCapture();
    }
}
