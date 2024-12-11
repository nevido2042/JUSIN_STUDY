#include "pch.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ZoomMgr.h"

CTile::CTile()
{
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;
}

int CTile::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

int CTile::Late_Update()
{
    
	return OBJ_NOEVENT;
}

void CTile::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Ice");

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	float     fZoom = CZoomMgr::Get_Instance()->Get_Zoom();

	// 크기를 절반으로 줄임
	int newWidth = TILE_SIZE * fZoom;  // 128의 절반
	int newHeight = TILE_SIZE * fZoom; // 128의 절반

	BitBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		newWidth,
		newWidth,
		hMemDC,
		0,
		0,
		SRCCOPY);
}

void CTile::Release()
{
}
