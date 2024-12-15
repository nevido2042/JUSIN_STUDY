#include "pch.h"
#include "BoltActionRifle.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"

CBoltActionRifle::CBoltActionRifle()
{
}

CBoltActionRifle::~CBoltActionRifle()
{
	Release();
}

void CBoltActionRifle::Fire()
{
    if (!Try_Fire())
    {
        return;
    }

    CObj* pObj = CAbstractFactory<CBullet_Small>::Create(m_tInfo.fX, m_tInfo.fY, m_fAngle);
    CObjMgr::Get_Instance()->Add_Object(OBJ_PROJECTILE, pObj);
}

void CBoltActionRifle::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    //무기 출력
    HDC hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"BoltActionRifle");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        64, 64,
        hTestDC, 0, 0, 64, 64,
        RGB_WHITE);
}
