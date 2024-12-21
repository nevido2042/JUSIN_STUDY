#include "pch.h"
#include "DetailView.h"
#include "BmpMgr.h"

void CDetailView::Initialize()
{
	Set_Size(400.f, 150.f);

	m_eRenderID = RENDER_UI;
}

int CDetailView::Update()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CDetailView::Late_Update()
{
}

void CDetailView::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"DetailViewBG");

	BitBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0, 0,
		SRCCOPY);
}

void CDetailView::Release()
{
}
