#include "pch.h"
#include "MyButton.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"

CMyButton::CMyButton()
{
}

CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize()
{
	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 64.f;	

}

int CMyButton::Update()
{

	__super::Update_Rect();


	return OBJ_NOEVENT;
}

int CMyButton::Late_Update()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&m_tRect, ptMouse))
	{
		if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
		{
			m_iDrawID = 2;

			if (!lstrcmp(L"Start", m_pImgKey))
				CSceneMgr::Get_Instance()->Set_Scene(SC_COLONY);

			else if (!lstrcmp(L"Edit", m_pImgKey))
				CSceneMgr::Get_Instance()->Set_Scene(SC_EDIT);

			else if (!lstrcmp(L"Exit", m_pImgKey))
				DestroyWindow(g_hWnd);

			return OBJ_SETSCENE;
		}

		m_iDrawID = 1;
	}

	else
		m_iDrawID = 0;
}

void CMyButton::Render(HDC hDC)
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

void CMyButton::Release()
{
}
