#include "pch.h"
#include "BoxMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"

CBoxMgr* CBoxMgr::m_pInstance = nullptr;

CBoxMgr::CBoxMgr()
{
	ZeroMemory(&m_tBoxPoint, sizeof(m_tBoxPoint));
}

CBoxMgr::~CBoxMgr()
{
	Release();
}

void CBoxMgr::Initialize()
{
	/*LINEPOINT LT{ 0,0 };
	LINEPOINT RT{ 100,0 };
	LINEPOINT LB{ 0,100 };
	LINEPOINT RB{ 100,100 };

	CBox* pBox = new CBox(LT, RT, LB, RB);
	m_Boxlist.push_back(pBox);*/
}

int CBoxMgr::Update()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if ((!m_tBoxPoint[HEAD].fX) && (!m_tBoxPoint[HEAD].fY))
		{
			m_tBoxPoint[HEAD].fX = (float)ptMouse.x;
			m_tBoxPoint[HEAD].fY = (float)ptMouse.y;
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		m_tBoxPoint[TAIL].fX = (float)ptMouse.x;
		m_tBoxPoint[TAIL].fY = (float)ptMouse.y;
	}

	if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
	{
		ZeroMemory(&m_tBoxPoint, sizeof(m_tBoxPoint));
	}

	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		Save_Box();
		return 0;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('L'))
	{
		Load_Box();
		return 0;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);

	return 0;
}

void CBoxMgr::Late_Update()
{
}

void CBoxMgr::Render(HDC hDC)
{
	for (auto& pLine : m_Boxlist)
		pLine->Render(hDC);

	MoveToEx(hDC, m_tBoxPoint[HEAD].fX, m_tBoxPoint[HEAD].fY, nullptr);
	LineTo(hDC, m_tBoxPoint[TAIL].fX, m_tBoxPoint[TAIL].fY);
}

void CBoxMgr::Release()
{
	for_each(m_Boxlist.begin(), m_Boxlist.end(), Safe_Delete<CBox*>);
	m_Boxlist.clear();
}

void CBoxMgr::Save_Box()
{
}

void CBoxMgr::Load_Box()
{
}
