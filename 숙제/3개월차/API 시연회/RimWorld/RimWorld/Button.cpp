#include "pch.h"
#include "Button.h"
#include "KeyMgr.h"
#include "BmpMgr.h"

CButton::CButton()
{
}

CButton::~CButton()
{
	Release();
}

void CButton::Initialize()
{
}

int CButton::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();


	return OBJ_NOEVENT;
}

void CButton::Late_Update()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&m_tRect, ptMouse))
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			return;
		}

		m_iDrawID = 1;
	}

	else
		m_iDrawID = 0;
}

void CButton::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	BitBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		m_iDrawID * (int)m_tInfo.fCX,
		0,
		SRCCOPY);
}

void CButton::Release()
{
}
