#include "pch.h"
#include "Bullet_Small.h"
#include "BmpMgr.h"


CBullet_Small::CBullet_Small()
{
}

CBullet_Small::~CBullet_Small()
{
	Release();
}

void CBullet_Small::Render(HDC hDC)
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

    ////ÃÑ¾Ë Ãâ·Â
    //HDC hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Bullet_Small");
    //GdiTransparentBlt(hDC,
    //    m_tRect.left + iScrollX,
    //    m_tRect.top + iScrollY,
    //    64, 64,
    //    hTestDC, 0, 0, 64, 64,
    //    RGB_PURPLE);
}
