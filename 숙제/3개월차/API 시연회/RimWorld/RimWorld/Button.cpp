#include "pch.h"
#include "Button.h"
#include "KeyMgr.h"
#include "BmpMgr.h"

CButton::CButton()
	:m_pParent(nullptr), m_bActivate(false)
{
}

CButton::~CButton()
{
	Release();
}

void CButton::On_Click()
{
}

void CButton::Set_Activate(bool _bActivate)
{
	m_bActivate = _bActivate;
}

void CButton::Initialize()
{
}

int CButton::Update()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	if (!m_bActivate)
	{
		return OBJ_NOEVENT;
	}

	if (Is_MouseHovered())
	{

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			On_Click();
			return OBJ_NOEVENT;
		}

		m_iDrawID = 1;
	}
	else
	{
		m_iDrawID = 0;
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CButton::Late_Update()
{
	
}

void CButton::Render(HDC hDC)
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
		m_iDrawID * (int)m_tInfo.fCX,
		0,
		SRCCOPY);
}

void CButton::Release()
{
}
