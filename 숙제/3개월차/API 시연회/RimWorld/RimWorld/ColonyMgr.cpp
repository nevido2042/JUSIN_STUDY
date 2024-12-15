#include "pch.h"
#include "ColonyMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "Rim.h"
#include "TimeMgr.h"

CColonyMgr* CColonyMgr::m_pInstance = nullptr;

CColonyMgr::CColonyMgr()
	:m_pTarget(nullptr)
{
}

CColonyMgr::~CColonyMgr()
{
    Release();
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
}

void CColonyMgr::Initialize()
{
    
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

            //Ÿ���� Ŭ�������� �̵���Ű��
            static_cast<CRim*>(m_pTarget)->
                Move_To(POS{ fX, fY });
        }

        ReleaseCapture();
    }
}

void CColonyMgr::Render(HDC hDC)
{
}

void CColonyMgr::Release()
{
}
