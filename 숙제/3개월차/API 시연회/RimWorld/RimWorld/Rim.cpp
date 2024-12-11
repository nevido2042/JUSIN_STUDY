#include "pch.h"
#include "Rim.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CRim::CRim()
{
}

CRim::~CRim()
{
    Release();
}

void CRim::Initialize()
{
    //m_tInfo.fCX = 128.f;
    //m_tInfo.fCY = 152.f;
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;
}

int CRim::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CRim::Late_Update()
{

}

void CRim::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, 
        m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

    // ¸² ¸öÅë, ¾ó±¼, ¸Ó¸® ¼ø Å×½ºÆ®
    HDC hTestDC;
    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_south");

    // ¸öÅë
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX - IMAGE_OFFSET_X
        , m_tRect.top + iScrollY - IMG_OFFSET_Y,
        128, 128,
        hTestDC, 0, 0, 128, 128,  
        RGB_PURPLE);


    //¾ó±¼
    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_south");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX - IMAGE_OFFSET_X,
        m_tRect.top + iScrollY - HEAD_OFFSET - IMG_OFFSET_Y,
        128, 128,
        hTestDC, 0, 0, 128, 128,
        RGB_PURPLE);

    // ¸Ó¸®
    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Afro_south");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX - IMAGE_OFFSET_X,
        m_tRect.top + iScrollY - HEAD_OFFSET - IMG_OFFSET_Y,
        128, 128,
        hTestDC, 0, 0, 128, 128,
        RGB_PURPLE);
}

void CRim::Release()
{
}
