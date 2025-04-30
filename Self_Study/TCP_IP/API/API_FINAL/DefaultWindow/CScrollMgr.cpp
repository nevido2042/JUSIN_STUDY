#include "pch.h"
#include "CScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr() : m_fScrollX(0.f), m_fScrollY(0.f)
{
}

CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Scroll_Lock()
{
	if (0.f < m_fScrollX)
		m_fScrollX = 0.f;

	if (WINCX - 1920.f > m_fScrollX)
		m_fScrollX = WINCX - 1920.f;

	if (0.f < m_fScrollY)
		m_fScrollY = 0.f;

	if (WINCY - 1280.f > m_fScrollY)
		m_fScrollY = WINCY - 1280.f;
}
