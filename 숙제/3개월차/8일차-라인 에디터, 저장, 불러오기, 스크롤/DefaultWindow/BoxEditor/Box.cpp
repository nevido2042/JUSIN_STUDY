#include "pch.h"
#include "Box.h"
#include "CScrollMgr.h"

CBox::CBox()
{
}

CBox::CBox(LINEPOINT& _tLT, LINEPOINT& _tRT, LINEPOINT& _tLB, LINEPOINT& _tRB)
	:m_tInfo(_tLT, _tRT, _tLB, _tRB)
{
}

CBox::CBox(BOX& _tInfo)
	:m_tInfo(_tInfo)
{
}

CBox::~CBox()
{
}

void CBox::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	MoveToEx(hDC, (int)m_tInfo.tLT.fX + iScrollX, (int)m_tInfo.tLT.fY, nullptr);
	LineTo(hDC, (int)m_tInfo.tRT.fX + iScrollX, (int)m_tInfo.tRT.fY);
	LineTo(hDC, (int)m_tInfo.tRB.fX + iScrollX, (int)m_tInfo.tRB.fY);
	LineTo(hDC, (int)m_tInfo.tLB.fX + iScrollX, (int)m_tInfo.tLB.fY);
}
