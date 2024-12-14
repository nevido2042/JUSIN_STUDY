#include "pch.h"
#include "ChargeBlasterLight.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CChargeBlasterLight::CChargeBlasterLight()
{
}

CChargeBlasterLight::~CChargeBlasterLight()
{
}

void CChargeBlasterLight::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    //무기 출력
    HDC hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"ChargeBlasterLight");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        64, 64,
        hTestDC, 0, 0, 64, 64,
        RGB_WHITE);
}
