#include "pch.h"
#include "Charge_Small.h"
#include "BmpMgr.h"


CCharge_Small::CCharge_Small()
{
}

CCharge_Small::~CCharge_Small()
{
	Release();
}

void CCharge_Small::Initialize()
{
    CProjectile::Initialize();
    m_fDamage = 3.f;
}

void CCharge_Small::Render(HDC hDC)
{
    CProjectile::Render(hDC);

    /*int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    Ellipse(hDC, 
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        m_tRect.right + iScrollX,
        m_tRect.bottom + iScrollY
    );*/

    ////�Ѿ� ���
    //HDC hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Charge_Small");
    //GdiTransparentBlt(hDC,
    //    m_tRect.left + iScrollX,
    //    m_tRect.top + iScrollY,
    //    64, 64,
    //    hTestDC, 0, 0, 64, 64,
    //    RGB_WHITE);
}
