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
	m_eRenderID = RENDER_UI;
}

int CMyButton::Update()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CMyButton::Late_Update()
{
	if (Is_MouseHovered())
	{
		m_iDrawID = 1;
		if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
		{
			if (!lstrcmp(L"Start", m_pImgKey))
			{
				CSceneMgr::Get_Instance()->Set_Scene(SC_COLONY);

			}
			else if (!lstrcmp(L"Exit", m_pImgKey))
			{
				DestroyWindow(g_hWnd);
			}
			else if (!lstrcmp(L"ButtonSubtleAtlas_Architect", m_pImgKey))
			{
				DestroyWindow(g_hWnd);
			}
			

			return;
		}
	}
	else 
	{
		m_iDrawID = 0;
	}

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
