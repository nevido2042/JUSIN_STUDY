#include "pch.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CTile::CTile() : m_eOption(OPT_END)
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

	m_eOption = OPT_REACHABLE;
}

int CTile::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CTile::Late_Update()
{

}

void CTile::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	BitBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		TILECX,
		TILECY,
		hMemDC,
		TILECX * m_iDrawID,
		0,
		SRCCOPY);

	// 사각형 그리기
	MoveToEx(hDC, m_tRect.left, m_tRect.top, nullptr);         // 왼쪽 위로 이동
	LineTo(hDC, m_tRect.right, m_tRect.top);                  // 상단 선
	LineTo(hDC, m_tRect.right, m_tRect.bottom);               // 오른쪽 선
	LineTo(hDC, m_tRect.left, m_tRect.bottom);                // 하단 선
	LineTo(hDC, m_tRect.left, m_tRect.top);                   // 왼쪽 선

}

void CTile::Release()
{
}
