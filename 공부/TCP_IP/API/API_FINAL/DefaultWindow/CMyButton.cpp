#include "pch.h"
#include "CMyButton.h"
#include "CBmpMgr.h"
#include "CSceneMgr.h"
#include "CKeyMgr.h"

CMyButton::CMyButton() : m_iDrawID(0)
{
}

CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize()
{
	m_tInfo.fCX = 150.f;
	m_tInfo.fCY = 150.f;	

	m_eRender = RENDER_UI;
}

int CMyButton::Update()
{

	__super::Update_Rect();


	return OBJ_NOEVENT;
}

void CMyButton::Late_Update()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&m_tRect, ptMouse))
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			if (!lstrcmp(L"Start", m_pImgKey))
				CSceneMgr::Get_Instance()->Set_Scene(SC_STAGE);

			else if (!lstrcmp(L"Edit", m_pImgKey))
				CSceneMgr::Get_Instance()->Set_Scene(SC_EDIT);

			else if (!lstrcmp(L"Exit", m_pImgKey))
				DestroyWindow(g_hWnd);

			return;
		}

		m_iDrawID = 1;
	}

	else
		m_iDrawID = 0;
}

void CMyButton::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	
	GdiTransparentBlt(hDC,			
						m_tRect.left,	
						m_tRect.top,
						(int)m_tInfo.fCX,	
						(int)m_tInfo.fCY,
						hMemDC,				
						m_iDrawID * (int)m_tInfo.fCX,
						0,
						(int)m_tInfo.fCX,	
						(int)m_tInfo.fCY,
						RGB(255, 255, 255));
}

void CMyButton::Release()
{
}
