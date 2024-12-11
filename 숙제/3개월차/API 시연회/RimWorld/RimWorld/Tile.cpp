#include "pch.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ZoomMgr.h"

CTile::CTile() : m_iDrawID(0), m_iOption(0)
{
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;

	m_pImgKey = L"Ice";

}

int CTile::Update()
{

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

int CTile::Late_Update()
{
	return OBJ_NOEVENT;
}

void CTile::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	float	fZoom = CZoomMgr::Get_Instance()->Get_Zoom();

	BitBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		TILECX * fZoom,
		TILECY * fZoom,
		hMemDC,
		TILECX * m_iDrawID,
		0,
		SRCCOPY);

}

void CTile::Release()
{
}
