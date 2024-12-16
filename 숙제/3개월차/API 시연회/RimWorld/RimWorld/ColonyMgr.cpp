#include "pch.h"
#include "ColonyMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "Rim.h"
#include "TimeMgr.h"
#include "BmpMgr.h"

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
        //Ÿ���� ���ְ�,
        //���콺�� ��ü ��� �������� ����.
        //���콺�� ��ġ�ϴ� Ÿ�Ͽ� ����ǥ�ø� ����.
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
    
    //esc ���� �� ���ø��� ���ư���
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
            //���콺 ��ǥ�� �� ��Ƽ� �־�� �Ѵ�.

            float fX = float(ptMouse.x - iScrollX);
            float fY = float(ptMouse.y - iScrollY);

            fX = float(fX -(int)fX % TILECX + TILECX * 0.5f);
            fY = float(fY -(int)fY % TILECY + TILECY * 0.5f);
            //Pawn�� Ŭ�������� Ÿ������ �����ϰ� ���ݸ�� ��������
            //Ÿ���� Ŭ�������� �̵���Ű��, ���ݸ�� ���, Ÿ�� ����
            CRim* pTargetRim = static_cast<CRim*>(m_pTarget);
            
            //�׺������ ���̾�����
            if (pTargetRim->Get_IsNavigating())
            {
                //�׺������ ���� ��û�� ������.
                pTargetRim->RequestNavStop();
            }
            pTargetRim->Move_To(POS{ fX, fY });
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

void CColonyMgr::Render(HDC hDC)
{
    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Deconstruct_mini");

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    //��ü ����Ʈ�� �ִ� ��� ���鿡 ��ü ������ ǥ��
    for (CObj* pObj : m_DeconstructSet)
    {
        GdiTransparentBlt(hDC,
            (int)pObj->Get_Info().fX + iScrollX - IMAGE_OFFSET_X,
            (int)pObj->Get_Info().fY + iScrollY - IMAGE_OFFSET_Y,
            64,
            64,
            hMemDC,
            0, 0,
            64,
            64,
            RGB_WHITE);
    }

    //��ü ����� ��� ���콺�� ��ü �׸� ǥ��
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
            hMemDC,
            0, 0,
            64,
            64,
            RGB_WHITE);
    }
}

void CColonyMgr::Release()
{
}
