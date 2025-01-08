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
#include "TutorialMgr.h"
#include "Breakable.h"

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
    //������, ��� ��ư�� �����´�.
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

    //���ּ� �Ǽ� ��ư
    CObj* pShip = CAbstractFactory<CMyButton>::
        Create(fShortBtnCX * 4.5f, WINCY - fShortBtnCY * 1.f);
    pShip->Set_Size(fShortBtnCX, fShortBtnCY);
    pShip->Set_ImgKey(L"ShipBtn");
    pStructureBtn->Get_ChildList()->push_back(pShip);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pShip);

    //���ּ� �߻� ��ư
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
        //Ÿ���� ���ְ�,
        //���콺�� ��ü ��� �������� ����.
        //���콺�� ��ġ�ϴ� Ÿ�Ͽ� ����ǥ�ø� ����.
        m_pTarget = nullptr;
        break;
    case CColonyMgr::MODE_CONSTRUCT:
        //Ÿ���� ���ְ�,
        //���콺�� ö�� ��� �������� ����.
        //���콺�� ��ġ�ϴ� Ÿ�Ͽ� ����ǥ�ø� ����.
        m_pTarget = nullptr;
        break;
    case CColonyMgr::MODE_SHIP:
        //Ÿ���� ���ְ�,
        //���콺�� ö�� ��� �������� ����.
        //���콺�� ��ġ�ϴ� Ÿ�Ͽ� ����ǥ�ø� ����.
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

    static_cast<CBreakable*>(_tTask.pObj)->Set_TaskReserved(true);

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

    static_cast<CBreakable*>(_tTask.pObj)->Set_TaskReserved(true);

    CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
    CSoundMgr::Get_Instance()->PlaySound(L"Click2.wav", SOUND_UI, .5f);

    Notify_TaskChange();
}

void CColonyMgr::Emplace_ConstructSet(TASK _tTask)
{
    //�Ǽ� ��Ͽ� �ֱ��� ö �Ɵ����� �����ϴ� �� Ȯ�� ��
    //ö �������� ������ ���Set���� �ֱ�

    auto Result = m_ConstructSet.emplace(_tTask);
    //���� �����ϸ� ����
    if (!Result.second)
    {
        return;
    }

    CTile* pTile = static_cast<CTile*>(_tTask.pObj);

    switch (_tTask.eType)
    {
    case TASK::TYPE::WALL:
        pTile->Set_Reserved(CTile::RESERVED_WALL);
        break;
    case TASK::TYPE::CAMPFIRE:
        pTile->Set_Reserved(CTile::RESERVED_CAMPFIRE);
        break;
    case TASK::TYPE::SHIP:
        pTile->Set_Reserved(CTile::RESERVED_SHIP);
        break;
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
    //ȭ�� ��ũ��
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

    //���� ���ǵ� ����
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
    
    //R ���� �� �� ����/����� ���� ����
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
                    if (CTutorialMgr::Get_Instance()->Get_CurQuest() == CTutorialMgr::QUEST_DRAFTE)
                    {
                        CTutorialMgr::Get_Instance()->IncreaseQuestProgress();
                    }

                    CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
                    CSoundMgr::Get_Instance()->PlaySound(L"Drafted_1a.wav", SOUND_UI, .2f);
                    pTargetRim->Change_State(CRim::DRAFTED);
                    pTargetRim->PutDown_Item();
                }  
            }
        }
    }

    //esc ���� �� ���ø��� ���ư���
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
    HDC		hDeconstructDC = CBmpMgr::Get_Instance()->Find_Image(L"Deconstruct_mini");
    HDC		hSteelWallDC = CBmpMgr::Get_Instance()->Find_Image(L"RockSmooth_MenuIcon_mini");
    HDC		hShipDC = CBmpMgr::Get_Instance()->Find_Image(L"Ship");
    HDC     hCampfireBlueprintDC = CBmpMgr::Get_Instance()->Find_Image(L"CampfireBlueprint");

    // ������ �� ����
    if (m_pTarget)
    {
        float fZoom = CCamera::Get_Instance()->Get_Zoom();
        POINT tPoint = CCamera::Get_Instance()->WorldToScreen(
            (float)m_pTarget->Get_Rect()->left,
            (float)m_pTarget->Get_Rect()->top
        );

        HDC hSelectionBracketWholeDC = CBmpMgr::Get_Instance()->Find_Image(L"SelectionBracketWhole");

        GdiTransparentBlt(hDC,
            (int)tPoint.x,
            (int)tPoint.y,
            (int)(64 * fZoom),
            (int)(64 * fZoom),
            hSelectionBracketWholeDC,
            0, 0,
            64,
            64,
            RGB_PURPLE);
    }

    // ��ü ����� ��� ���콺�� ��ü �׸� ǥ��
    if (m_eMode == MODE_DECONSTRUCT)
    {
        //float fZoom = CCamera::Get_Instance()->Get_Zoom();
        POINT ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        int iconSize = (int)(64);

        GdiTransparentBlt(hDC,
            ptMouse.x,
            ptMouse.y,
            iconSize,
            iconSize,
            hDeconstructDC,
            0, 0,
            64,
            64,
            RGB_WHITE);

        if (m_bDrawRect)
        {
            HPEN hPen = CreatePen(PS_SOLID, (int)(2), RGB(0, 255, 0));
            HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

            MoveToEx(hDC, m_tSelectRect.left, m_tSelectRect.top, NULL);
            LineTo(hDC, m_tSelectRect.right, m_tSelectRect.top);
            LineTo(hDC, m_tSelectRect.right, m_tSelectRect.bottom);
            LineTo(hDC, m_tSelectRect.left, m_tSelectRect.bottom);
            LineTo(hDC, m_tSelectRect.left, m_tSelectRect.top);

            SelectObject(hDC, hOldPen);
            DeleteObject(hPen);
        }
    }

    // ���� ����� ��� ���콺�� ���� ��� �׸� ǥ��
    else if (m_eMode == MODE_LOGGING)
    {
        //float fZoom = CCamera::Get_Instance()->Get_Zoom();
        POINT ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        int iconSize = 48;

        GdiTransparentBlt(hDC,
            ptMouse.x,  // ���콺 �߽ɿ� ������ ǥ��
            ptMouse.y,
            iconSize,
            iconSize,
            hLoggingDC,
            0, 0,
            64,
            64,
            RGB_WHITE);

        if (m_bDrawRect)
        {
            HPEN hPen = CreatePen(PS_SOLID, (int)(2), RGB(0, 255, 0));
            HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

            MoveToEx(hDC, m_tSelectRect.left, m_tSelectRect.top, NULL);
            LineTo(hDC, m_tSelectRect.right, m_tSelectRect.top);
            LineTo(hDC, m_tSelectRect.right, m_tSelectRect.bottom);
            LineTo(hDC, m_tSelectRect.left, m_tSelectRect.bottom);
            LineTo(hDC, m_tSelectRect.left, m_tSelectRect.top);

            SelectObject(hDC, hOldPen);
            DeleteObject(hPen);
        }
    }

    // ������ ���
    else if (m_eMode == MODE_CONSTRUCT || m_eMode == MODE_SHIP || m_eMode == MODE_CAMPFIRE)
    {
        //float fZoom = CCamera::Get_Instance()->Get_Zoom();
        POINT ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        HDC hIconDC = nullptr;
        int iconSize(64);
        int iDCSize(64);

        if (m_eMode == MODE_CONSTRUCT)
        {
            hIconDC = hSteelWallDC;
            iconSize = (int)(32);
            iDCSize = 32;

        }
        else if (m_eMode == MODE_SHIP)
        {
            hIconDC = hShipDC;
            iconSize = 128;
            iDCSize = 512;
        }
        else if (m_eMode == MODE_CAMPFIRE)
        {
            hIconDC = hCampfireBlueprintDC;
        }

        GdiTransparentBlt(hDC,
            ptMouse.x,// - iconSize / 2,  // ���콺 �߽ɿ� ������ ǥ��
            ptMouse.y,// - iconSize / 2,
            iconSize,
            iconSize,
            hIconDC,
            0, 0,
            iDCSize, iDCSize,
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

        //���콺 �Է�
        SetCapture(g_hWnd);
        POINT	ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        RECT tClientRect;
        GetClientRect(g_hWnd, &tClientRect);

        if (PtInRect(&tClientRect, ptMouse))
        {
            CRim* pTargetRim = static_cast<CRim*>(m_pTarget);
            //���� Rim�� �������°� �ƴ϶�� ���������� �������� ���ϵ���
            if (pTargetRim->Get_State() != CRim::DRAFTED)
            {
                return;
            }
            //���콺 ��ǥ�� �� ��Ƽ� �־�� �Ѵ�.

            POINT tPoint = CCamera::Get_Instance()->ScreenToWorld(ptMouse.x, ptMouse.y);

            float fX = (float)tPoint.x;//float(ptMouse.x - iScrollX);
            float fY = (float)tPoint.y; //float(ptMouse.y - iScrollY);

            

            fX = float(fX - (int)fX % TILECX + TILECX * 0.5f);
            fY = float(fY - (int)fY % TILECY + TILECY * 0.5f);
            //Pawn�� Ŭ�������� Ÿ������ �����ϰ� ���ݸ�� ��������
            //Ÿ���� Ŭ�������� �̵���Ű��, ���ݸ�� ���, Ÿ�� ����


            //�׺������ ���̾�����
            //if (pTargetRim->Get_IsNavigating())
            //{
            //    //�׺������ ���� ��û�� ������.
            //    pTargetRim->RequestNavStop();
            //}
            CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
            CSoundMgr::Get_Instance()->PlaySound(L"TabOpen.wav", SOUND_UI, .2f);

            POS tMovePos{ (int)fX, (int)fY };
            pTargetRim->Move_To(tMovePos);

            //POINT tScreenPoint = CCamera::Get_Instance()->WorldToScreen(fX, fY);

            CEffectMgr::Get_Instance()->Create_Effect((float)fX, (float)fY, 64.f, 64.f, L"FeedbackGoto", 30.f);

            //���� ���� ���̾��� Ÿ�ڵ� �־��ٸ� ����
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
    //��ü ����� ��
    if (m_eMode == MODE_DECONSTRUCT)
    {
        POINT	ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        //m_bDrawRect = false;

        if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
        {
            m_bDrawRect = true;

            m_tSelectRect.right = ptMouse.x;
            m_tSelectRect.bottom = ptMouse.y;
        }

        if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
        {
            m_bDrawRect = false;

            ZeroMemory(&m_tSelectRect, sizeof(RECT));

            m_tSelectRect.left = ptMouse.x;
            m_tSelectRect.top = ptMouse.y;
        }
        else if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
        {
            m_bDrawRect = false;
            //�簢�� ���ο� �ִ� steelWall���� ��ü �۾����� �ִ´�.

            int iScrollX = -(int)CScrollMgr::Get_Instance()->Get_ScrollX();
            int iScrollY = -(int)CScrollMgr::Get_Instance()->Get_ScrollY();


            int iLeft = m_tSelectRect.left + iScrollX;
            int iRight = m_tSelectRect.right + iScrollX;
            int iTop = m_tSelectRect.top + iScrollY;
            int iBottom = m_tSelectRect.bottom + iScrollY;

            list<CObj*> steelWallList = CObjMgr::Get_Instance()->Get_List()[OBJ_WALL];
            for (CObj* pObj : steelWallList)
            {
                // pObj�� ���� ��ǥ�� ��ũ�� ��ǥ�� ��ȯ�մϴ�.
                POINT tScreenPos = CCamera::Get_Instance()->WorldToScreen(pObj->Get_Info().fX, pObj->Get_Info().fY);

                // ���� ������ ��ü�� ��ġ���� Ȯ��
                if (iLeft < iRight)
                {
                    if (iLeft > tScreenPos.x || iRight < tScreenPos.x)
                    {
                        continue;
                    }
                }
                else
                {
                    if (iLeft < tScreenPos.x || iRight > tScreenPos.x)
                    {
                        continue;
                    }
                }

                if (iTop < iBottom)
                {
                    if (iTop > tScreenPos.y || iBottom < tScreenPos.y)
                    {
                        continue;
                    }
                }
                else
                {
                    if (iTop < tScreenPos.y || iBottom > tScreenPos.y)
                    {
                        continue;
                    }
                }

                // �۾��� ��ü�� ��ü ��Ͽ� �߰�
                TASK tTask;
                tTask.pObj = pObj;
                CColonyMgr::Get_Instance()->Emplace_DeconstructSet(tTask);  // ��ü ��� �߰�

                ZeroMemory(&m_tSelectRect, sizeof(RECT));
            }
        }
    }
}

void CColonyMgr::MouseDrag_Select_Tree()
{
    //��ü ����� ��
    if (m_eMode == MODE_LOGGING)
    {
        POINT	ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        //m_bDrawRect = false;

        if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
        {
            m_bDrawRect = true;

            m_tSelectRect.right = ptMouse.x;
            m_tSelectRect.bottom = ptMouse.y;
        }

        if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
        {
            m_bDrawRect = false;

            ZeroMemory(&m_tSelectRect, sizeof(RECT));

            m_tSelectRect.left = ptMouse.x;
            m_tSelectRect.top = ptMouse.y;
        }
        else if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
        {
            m_bDrawRect = false;

            //�簢�� ���ο� �ִ� steelWall���� ��ü �۾����� �ִ´�.

            int iScrollX = -(int)CScrollMgr::Get_Instance()->Get_ScrollX();
            int iScrollY = -(int)CScrollMgr::Get_Instance()->Get_ScrollY();

            int iLeft = m_tSelectRect.left + iScrollX;
            int iRight = m_tSelectRect.right + iScrollX;
            int iTop = m_tSelectRect.top + iScrollY;
            int iBottom = m_tSelectRect.bottom + iScrollY;

            list<CObj*> TreeList = CObjMgr::Get_Instance()->Get_List()[OBJ_TREE];
            for (CObj* pObj : TreeList)
            {
                // pObj�� ���� ��ǥ�� ��ũ�� ��ǥ�� ��ȯ�մϴ�.
                POINT tScreenPos = CCamera::Get_Instance()->WorldToScreen(pObj->Get_Info().fX, pObj->Get_Info().fY);

                // ���� ������ ��ü�� ��ġ���� Ȯ��
                if (iLeft < iRight)
                {
                    if (iLeft > tScreenPos.x || iRight < tScreenPos.x)
                    {
                        continue;
                    }
                }
                else
                {
                    if (iLeft < tScreenPos.x || iRight > tScreenPos.x)
                    {
                        continue;
                    }
                }

                if (iTop < iBottom)
                {
                    if (iTop > tScreenPos.y || iBottom < tScreenPos.y)
                    {
                        continue;
                    }
                }
                else
                {
                    if (iTop < tScreenPos.y || iBottom > tScreenPos.y)
                    {
                        continue;
                    }
                }

                TASK tTask;
                tTask.pObj = pObj;
                CColonyMgr::Get_Instance()->Emplace_LoggingSet(tTask);//��ü ��� �߰�

                ZeroMemory(&m_tSelectRect, sizeof(RECT));
            }
        }
    }
}
