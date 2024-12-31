#include "pch.h"
#include "Panel.h"
#include "BmpMgr.h"

CPanel::CPanel()
{
}

CPanel::~CPanel()
{
    Release();
}

void CPanel::Initialize()
{
	m_eRenderID = RENDER_UI;
}

int CPanel::Update()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CPanel::Late_Update()
{
}

void CPanel::Render(HDC hDC)
{
	if (!m_bActivate)
	{
		return;
	}

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	BitBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0,
		0,
		SRCCOPY);
}

void CPanel::Release()
{
}
