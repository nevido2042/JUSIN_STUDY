#include "pch.h"
#include "RimSlot.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"

CRimSlot::CRimSlot()
{
}

CRimSlot::~CRimSlot()
{
	Release();
}

void CRimSlot::Initialize()
{
	Set_Size(64.f, 64.f);

	m_eRenderID = RENDER_UI;
}

int CRimSlot::Update()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CRimSlot::Late_Update()
{
	if (Is_MouseHovered())
	{
		if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
		{
			CScrollMgr::Get_Instance()->Set_ScrollX(-Get_Target()->Get_Info().fX + WINCX * 0.5f);
			CScrollMgr::Get_Instance()->Set_ScrollY(-Get_Target()->Get_Info().fY + WINCY * 0.5f);
		}
	}
}

void CRimSlot::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"DesButBG");

	BitBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0, 0,
		SRCCOPY);
}

void CRimSlot::Release()
{
}
