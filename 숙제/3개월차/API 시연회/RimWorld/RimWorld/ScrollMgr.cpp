#include "pch.h"
#include "ScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr() : m_fScrollX(0.f), m_fScrollY(0.f)
{
}

CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Scroll_Lock()
{
	float fMinX = TILECX * TILEX * 0.2f;
	float fMinY = TILECY * TILEY * 0.2f;

	float fMaxX = TILECX * TILEX * 1.2f;
	float fMaxY = TILECY * TILEY * 1.2f;

	if (fMinX < m_fScrollX)
		m_fScrollX = fMinX;

	if (WINCX - fMaxX > m_fScrollX)
		m_fScrollX = WINCX - fMaxX;

	if (fMinY < m_fScrollY)
		m_fScrollY = fMinY;

	if (WINCY - fMaxY > m_fScrollY)
		m_fScrollY = WINCY - fMaxY;
}
