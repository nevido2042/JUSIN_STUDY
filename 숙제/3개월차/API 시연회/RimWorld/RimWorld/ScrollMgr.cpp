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
	float fMinX = 0.f;
	float fMinY = 0.f;

	float fMaxX = 1920.f;
	float fMaxY = 1080.f;

	if (fMinX < m_fScrollX)
		m_fScrollX = fMinX;

	if (WINCX - fMaxX > m_fScrollX)
		m_fScrollX = WINCX - fMaxX;

	if (fMinY < m_fScrollY)
		m_fScrollY = fMinY;

	if (WINCY - fMaxY > m_fScrollY)
		m_fScrollY = WINCY - fMaxY;
}
