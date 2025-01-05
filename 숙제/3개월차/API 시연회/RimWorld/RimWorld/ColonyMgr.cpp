#include "pch.h"
#include "ColonyMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "Rim.h"
#include "TimeMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"
#include "AbstractFactory.h"
#include "MyButton.h"
#include "EffectMgr.h"

CColonyMgr* CColonyMgr::m_pInstance = nullptr;

CColonyMgr::CColonyMgr()
	:m_pTarget(nullptr), m_eMode(MODE_END), m_bShipBtnActive(false),
    m_bDrawRect(false)
{
    ZeroMemory(&m_tSelectRect, sizeof(RECT));
}

CColonyMgr::~CColonyMgr()
{
    Release();
}

void CColonyMgr::Create_ShipBtn()
{
    float fShortBtnCX = 64.f;
    float fShortBtnCY = 64.f;

    CObj* pStructureBtn(nullptr);
    CObj* pCommandBtn(nullptr);
    //구조물, 명령 버튼을 가져온다.
    list<CObj*> UIList = CObjMgr::Get_Instance()->Get_List()[OBJ_UI];

    for (CObj* pUI : UIList)
    {
        if (!lstrcmp(L"ButtonSubtleAtlas_Structure", pUI->Get_ImgKey()))
        {
            pStructureBtn = pUI;
        }
        else if (!lstrcmp(L"ButtonSubtleAtlas_Command", pUI->Get_ImgKey()))
        {
            pCommandBtn = pUI;
        }
    }

    if (!pStructureBtn || !pCommandBtn)
    {
        return;
    }

    //우주선 건설 버튼
    CObj* pShip = CAbstractFactory<CMyButton>::
        Create(fShortBtnCX * 4.5f, WINCY - fShortBtnCY * 1.f);
    pShip->Set_Size(fShortBtnCX, fShortBtnCY);
    pShip->Set_ImgKey(L"ShipBtn");
    pStructureBtn->Get_ChildList()->push_back(pShip);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pShip);

    //우주선 발사 버튼
    CObj* pLaunchShip = CAbstractFactory<CMyButton>::
        Create(fShortBtnCX * 4.5f, WINCY - fShortBtnCY * 1.f);
    pLaunchShip->Set_Size(fShortBtnCX, fShortBtnCY);
    pLaunchShip->Set_ImgKey(L"LaunchShipBtn");
    pCommandBtn->Get_ChildList()->push_back(pLaunchShip);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pLaunchShip);
}

void CColonyMgr::Change_Mode(MODE _eMode)
{
    switch (_eMode)
    {
    case CColonyMgr::MODE_SELECT:
        m_pTarget = nullptr;
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
    case CColonyMgr::MODE_SHIP:
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

void CColonyMgr::Emplace_LoggingSet(TASK _tTask)
{
    auto Result = m_LoggingSet.emplace(_tTask);

    if (!Result.second)
    {
        return;
    }

    CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
    CSoundMgr::Get_Instance()->PlaySound(L"Click2.wav", SOUND_UI, .5f);

    Notify_TaskChange();
}

void CColonyMgr::Emplace_DeconstructSet(TASK _tTask)
{
    auto Result = m_DeconstructSet.emplace(_tTask);

    if (!Result.second)
    {
        return;
    }

    CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
    CSoundMgr::Get_Instance()->PlaySound(L"Click2.wav", SOUND_UI, .5f);

    Notify_TaskChange();
}

void CColonyMgr::Emplace_ConstructSet(TASK _tTask)
{
    //건설 목록에 넣기전 철 아잍템이 존재하는 지 확인 후
    //철 아이템이 없으면 운반Set으로 넣기

    auto Result = m_ConstructSet.emplace(_tTask);
    //삽입 실패하면 리턴
    if (!Result.second)
    {
        return;
    }

    CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
    CSoundMgr::Get_Instance()->PlaySound(L"DragBuilding.wav", SOUND_UI, .2f);

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

void CColonyMgr::Set_Target(CObj* _pObj)
{
    m_pTarget = _pObj;

    CObj* pDetailView = CObjMgr::Get_Instance()->Find_Obj(L"DetailViewBG", OBJ_UI);
   
    pDetailView->Set_Activate(true);
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
        CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
        CSoundMgr::Get_Instance()->PlaySound(L"ClockTickingNormal.wav", SOUND_UI, 1.f);

        CTimeMgr::Get_Instance()->Set_GameSpeed(1.f);
    }
    else if (CKeyMgr::Get_Instance()->Key_Down('2'))
    {
        CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
        CSoundMgr::Get_Instance()->PlaySound(L"ClockTickingFast.wav", SOUND_UI, 1.f);

        CTimeMgr::Get_Instance()->Set_GameSpeed(2.f);
    }
    else if (CKeyMgr::Get_Instance()->Key_Down('3'))
    {
        CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
        CSoundMgr::Get_Instance()->PlaySound(L"ClockTickingSuperFast.wav", SOUND_UI, 1.f);

        CTimeMgr::Get_Instance()->Set_GameSpeed(3.f);
    }
    else if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
    {

        if (GAMESPEED == 0.f)
        {
            CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
            CSoundMgr::Get_Instance()->PlaySound(L"ClockTickingNormal.wav", SOUND_UI, 1.f);

            CTimeMgr::Get_Instance()->Set_GameSpeed(1.f);
        }
        else
        {
            CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
            CSoundMgr::Get_Instance()->PlaySound(L"ClockStops.wav", SOUND_UI, 1.f);

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
                if (pTargetRim->Get_IsDead())
                {
                    return;
                }

                if (pTargetRim->Get_State() == CRim::DRAFTED)
                {
                    CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
                    CSoundMgr::Get_Instance()->PlaySound(L"DraftOff.wav", SOUND_UI, .2f);
                    pTargetRim->Change_State(CRim::WANDERING);
                }
                else
                {
                    CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
                    CSoundMgr::Get_Instance()->PlaySound(L"Drafted_1a.wav", SOUND_UI, .2f);
                    pTargetRim->Change_State(CRim::DRAFTED);
                    pTargetRim->PutDown_Item();
                }  
            }
        }
    }

    //esc 누를 시 선택모드로 돌아가기
    if (CKeyMgr::Get_Instance()->Key_Down(VK_ESCAPE))
    {
        Change_Mode(MODE_SELECT);

        CObj* pDetailView = CObjMgr::Get_Instance()->Find_Obj(L"DetailViewBG", OBJ_UI);

        pDetailView->Set_Activate(false);
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

    MouseDrag_Select();
    
}

void CColonyMgr::Render(HDC hDC)
{
    HDC		hLoggingDC = CBmpMgr::Get_Instance()->Find_Image(L"HarvestWood");
    HDC		hCutPlantDC = CBmpMgr::Get_Instance()->Find_Image(L"CutPlant");
    HDC		hDeconstructDC = CBmpMgr::Get_Instance()->Find_Image(L"Deconstruct_mini");
    HDC		hSteelWallDC = CBmpMgr::Get_Instance()->Find_Image(L"RockSmooth_MenuIcon_mini");
    HDC		hShipDC = CBmpMgr::Get_Instance()->Find_Image(L"Ship");
    HDC     hCampfireDC = CBmpMgr::Get_Instance()->Find_Image(L"Campfire_MenuIcon");
    HDC     hCampfireBlueprintDC = CBmpMgr::Get_Instance()->Find_Image(L"CampfireBlueprint");


    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    //선택한 것 강조
    if (m_pTarget)
    {
        HDC		hSelectionBracketWholeDC = CBmpMgr::Get_Instance()->Find_Image(L"SelectionBracketWhole");

        GdiTransparentBlt(hDC,
            (int)m_pTarget->Get_Rect()->left + iScrollX,
            (int)m_pTarget->Get_Rect()->top + iScrollY,
            64,
            64,
            hSelectionBracketWholeDC,
            0, 0,
            64,
            64,
            RGB_PURPLE);
    }

    //해체 리스트에 있는 모든 벽들에 해체 아이콘 표시
    for (TASK tTask : m_DeconstructSet)
    {
        CObj* pObj = tTask.pObj;

        GdiTransparentBlt(hDC,
            (int)pObj->Get_Rect()->left + iScrollX,
            (int)pObj->Get_Rect()->top + iScrollY,
            64,
            64,
            hDeconstructDC,
            0, 0,
            64,
            64,
            RGB_WHITE);
    }
    //벌목 리스트에 있는 모든 벽들에 해체 아이콘 표시
    for (TASK tTask : m_LoggingSet)
    {
        CObj* pObj = tTask.pObj;

        GdiTransparentBlt(hDC,
            (int)pObj->Get_Rect()->left + iScrollX,
            (int)pObj->Get_Rect()->top + iScrollY,
            64,
            64,
            hCutPlantDC,
            0, 0,
            64,
            64,
            RGB_WHITE);
    }
    //건설할 벽들 표시, 우주선 표시
    for (TASK tTask : m_ConstructSet)
    {
        CObj* pObj = tTask.pObj;

        if (tTask.eType == TASK::WALL)
        {
            BitBlt(hDC,
                (int)pObj->Get_Rect()->left + iScrollX + 16,
                (int)pObj->Get_Rect()->top + iScrollY + 16,
                32,
                32,
                hSteelWallDC,
                0, 0,
                SRCCOPY);
        }
        else if (tTask.eType == TASK::SHIP)
        {
            GdiTransparentBlt(hDC,
                (int)pObj->Get_Rect()->left + iScrollX,
                (int)pObj->Get_Rect()->top + iScrollY,
                128,
                128,
                hShipDC,
                0, 0,
                512, 512,
                RGB_WHITE);
        }
        else if (tTask.eType == TASK::CAMPFIRE)
        {
            GdiTransparentBlt(hDC,
                (int)pObj->Get_Rect()->left + iScrollX,
                (int)pObj->Get_Rect()->top + iScrollY,
                64,
                64,
                hCampfireBlueprintDC,
                0, 0,
                64, 64,
                RGB_WHITE);
        }

        
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

        if (m_bDrawRect)
        {
            // 초록색 펜 생성 (두께 5)
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0)); // 두께 5, 색상 초록색
            HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

            // 사각형을 이루는 네 개의 선 그리기
            MoveToEx(hDC, m_tSelectRect.left, m_tSelectRect.top, NULL); // 시작점
            LineTo(hDC, m_tSelectRect.right, m_tSelectRect.top);        // 윗변
            LineTo(hDC, m_tSelectRect.right, m_tSelectRect.bottom);     // 오른쪽 변
            LineTo(hDC, m_tSelectRect.left, m_tSelectRect.bottom);      // 아랫변
            LineTo(hDC, m_tSelectRect.left, m_tSelectRect.top);         // 왼쪽 변 (닫기)

            // 펜 복원 및 삭제
            SelectObject(hDC, hOldPen);
            DeleteObject(hPen);

        }

    }
    //벌목 모드일 경우 마우스에 도끼모양 그림 표시
    else if (m_eMode == MODE_LOGGING)
    {
        POINT	ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        GdiTransparentBlt(hDC,
            ptMouse.x,
            ptMouse.y,
            64,
            64,
            hLoggingDC,
            0, 0,
            64,
            64,
            RGB_WHITE);

        if (m_bDrawRect)
        {
            // 초록색 펜 생성 (두께 5)
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0)); // 두께 5, 색상 초록색
            HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

            // 사각형을 이루는 네 개의 선 그리기
            MoveToEx(hDC, m_tSelectRect.left, m_tSelectRect.top, NULL); // 시작점
            LineTo(hDC, m_tSelectRect.right, m_tSelectRect.top);        // 윗변
            LineTo(hDC, m_tSelectRect.right, m_tSelectRect.bottom);     // 오른쪽 변
            LineTo(hDC, m_tSelectRect.left, m_tSelectRect.bottom);      // 아랫변
            LineTo(hDC, m_tSelectRect.left, m_tSelectRect.top);         // 왼쪽 변 (닫기)

            // 펜 복원 및 삭제
            SelectObject(hDC, hOldPen);
            DeleteObject(hPen);

        }

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
    else if (m_eMode == MODE_SHIP)
    {
        POINT	ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        GdiTransparentBlt(hDC,
            ptMouse.x,
            ptMouse.y,
            128,
            128,
            hShipDC,
            0, 0,
            512, 512,
            RGB_WHITE);
    }
    else if (m_eMode == MODE_CAMPFIRE)
    {
        POINT	ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        GdiTransparentBlt(hDC,
            ptMouse.x,
            ptMouse.y,
            64,
            64,
            hCampfireDC,
            0, 0,
            64, 64,
            RGB_WHITE);
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
            CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
            CSoundMgr::Get_Instance()->PlaySound(L"TabOpen.wav", SOUND_UI, .2f);

            POS tMovePos{ (int)fX, (int)fY };
            pTargetRim->Move_To(tMovePos);
            CEffectMgr::Get_Instance()->Create_Effect(tMovePos, 64.f, 64.f, L"FeedbackGoto", 30.f);

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

void CColonyMgr::MouseDrag_Select()
{
    MouseDrag_Select_Wall();
    MouseDrag_Select_Tree();
}

void CColonyMgr::MouseDrag_Select_Wall()
{
    //해체 모드일 때
    if (m_eMode == MODE_DECONSTRUCT)
    {
        POINT	ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        m_bDrawRect = false;

        if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
        {
            ZeroMemory(&m_tSelectRect, sizeof(RECT));

            m_tSelectRect.left = ptMouse.x;
            m_tSelectRect.top = ptMouse.y;
        }
        if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
        {
            m_bDrawRect = true;

            m_tSelectRect.right = ptMouse.x;
            m_tSelectRect.bottom = ptMouse.y;
        }
        if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
        {

            //사각형 내부에 있는 steelWall들을 해체 작업으로 넣는다.

            int iScrollX = -(int)CScrollMgr::Get_Instance()->Get_ScrollX();
            int iScrollY = -(int)CScrollMgr::Get_Instance()->Get_ScrollY();

            int iLeft = m_tSelectRect.left + iScrollX;
            int iRight = m_tSelectRect.right + iScrollX;
            int iTop = m_tSelectRect.top + iScrollY;
            int iBottom = m_tSelectRect.bottom + iScrollY;

            list<CObj*> steelWallList = CObjMgr::Get_Instance()->Get_List()[OBJ_WALL];
            for (CObj* pObj : steelWallList)
            {
                if (iLeft < iRight)
                {
                    //left, rigth 사이, top, down 사이에 있으면 내부에 있음
                    if (iLeft > pObj->Get_Info().fX || iRight < pObj->Get_Info().fX)
                    {
                        continue;
                    }
                }
                else
                {
                    //left, rigth 사이, top, down 사이에 있으면 내부에 있음
                    if (iLeft < pObj->Get_Info().fX || iRight > pObj->Get_Info().fX)
                    {
                        continue;
                    }
                }

                if (iTop < iBottom)
                {
                    if (iTop > pObj->Get_Info().fY || iBottom < pObj->Get_Info().fY)
                    {
                        continue;
                    }
                }
                else
                {
                    if (iTop < pObj->Get_Info().fY || iBottom > pObj->Get_Info().fY)
                    {
                        continue;
                    }
                }



                TASK tTask;
                tTask.pObj = pObj;
                CColonyMgr::Get_Instance()->Emplace_DeconstructSet(tTask);//해체 목록 추가
            }
        }
    }
}

void CColonyMgr::MouseDrag_Select_Tree()
{
    //해체 모드일 때
    if (m_eMode == MODE_LOGGING)
    {
        POINT	ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        m_bDrawRect = false;

        if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
        {
            ZeroMemory(&m_tSelectRect, sizeof(RECT));

            m_tSelectRect.left = ptMouse.x;
            m_tSelectRect.top = ptMouse.y;
        }
        if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
        {
            m_bDrawRect = true;

            m_tSelectRect.right = ptMouse.x;
            m_tSelectRect.bottom = ptMouse.y;
        }
        if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
        {

            //사각형 내부에 있는 steelWall들을 해체 작업으로 넣는다.

            int iScrollX = -(int)CScrollMgr::Get_Instance()->Get_ScrollX();
            int iScrollY = -(int)CScrollMgr::Get_Instance()->Get_ScrollY();

            int iLeft = m_tSelectRect.left + iScrollX;
            int iRight = m_tSelectRect.right + iScrollX;
            int iTop = m_tSelectRect.top + iScrollY;
            int iBottom = m_tSelectRect.bottom + iScrollY;

            list<CObj*> TreeList = CObjMgr::Get_Instance()->Get_List()[OBJ_TREE];
            for (CObj* pObj : TreeList)
            {
                if (iLeft < iRight)
                {
                    //left, rigth 사이, top, down 사이에 있으면 내부에 있음
                    if (iLeft > pObj->Get_Info().fX || iRight < pObj->Get_Info().fX)
                    {
                        continue;
                    }
                }
                else
                {
                    //left, rigth 사이, top, down 사이에 있으면 내부에 있음
                    if (iLeft < pObj->Get_Info().fX || iRight > pObj->Get_Info().fX)
                    {
                        continue;
                    }
                }

                if (iTop < iBottom)
                {
                    if (iTop > pObj->Get_Info().fY || iBottom < pObj->Get_Info().fY)
                    {
                        continue;
                    }
                }
                else
                {
                    if (iTop < pObj->Get_Info().fY || iBottom > pObj->Get_Info().fY)
                    {
                        continue;
                    }
                }



                TASK tTask;
                tTask.pObj = pObj;
                CColonyMgr::Get_Instance()->Emplace_LoggingSet(tTask);//해체 목록 추가
            }
        }
    }
}
