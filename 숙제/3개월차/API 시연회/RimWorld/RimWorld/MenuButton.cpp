#include "pch.h"
#include "MenuButton.h"
#include "BmpMgr.h"
#include "KeyMgr.h"

CMenuButton::CMenuButton()
{
}

CMenuButton::~CMenuButton()
{
	Release();
}

void CMenuButton::Initialize()
{
	Set_ImgKey(L"ButtonSubtleAtlas_Menu");

	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 32.f;

	m_tInfo.fX = WINCX - m_tInfo.fCX * 0.5f;
	m_tInfo.fY = WINCY - m_tInfo.fCY * 0.5f;
}

int CMenuButton::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();


	return OBJ_NOEVENT;
}

int CMenuButton::Late_Update()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&m_tRect, ptMouse))
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			m_iDrawID = 2;

			return OBJ_NOEVENT;
		}

		m_iDrawID = 1;
	}

	else
		m_iDrawID = 0;

	return OBJ_NOEVENT;
}

void CMenuButton::Render(HDC hDC)
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

void CMenuButton::Release()
{
}
