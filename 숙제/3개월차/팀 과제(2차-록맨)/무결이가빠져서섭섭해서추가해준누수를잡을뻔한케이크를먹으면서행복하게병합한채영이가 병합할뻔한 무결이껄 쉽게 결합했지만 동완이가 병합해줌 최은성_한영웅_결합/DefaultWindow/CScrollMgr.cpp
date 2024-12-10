#include "pch.h"
#include "CScrollMgr.h"


CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr() : m_fScrollX(0.f), m_fScrollY(0.f)
{
	m_fScrollSpeed =20.f ;
	m_eDir = (DIR_NONE);
	m_fCheck = 0;
	m_fScrollY = 0;
}

CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::SET_DIR(const DIRECTION& _Dir) // 스크롤 방향
{
	m_eDir = _Dir;

	switch (m_eDir)
	{
	case DIR_LEFT:
	case DIR_RIGHT:
		m_fCheck = m_fScrollX;
		break;
	case DIR_UP:
	case DIR_DOWN:
		m_fCheck = m_fScrollY;
		break;
	default:
		break;
	}

}

bool CScrollMgr::Update()
{
	switch (m_eDir)
	{
	case DIR_LEFT:
		m_fScrollX -= m_fScrollSpeed;
		if (m_fScrollX <= m_fCheck - WINCX)
		{
			m_fScrollX = m_fCheck - WINCX;
			m_eDir = DIR_NONE;
			return false;
		}
		return true;
	case DIR_RIGHT:
		m_fScrollX += m_fScrollSpeed;
		if (m_fScrollX >= m_fCheck + WINCX)
		{
			m_fScrollX = m_fCheck + WINCX;
			m_eDir = DIR_NONE;
			return false;
		}
		return true;
	case DIR_UP:
		m_fScrollY -= m_fScrollSpeed;
		if (m_fScrollY <= m_fCheck - WINCY)
		{
			m_fScrollY = m_fCheck - WINCY;
			m_eDir = DIR_NONE;
			return false;
		}
		return true;
	case DIR_DOWN:
		m_fScrollY += m_fScrollSpeed;
		if (m_fScrollY >= m_fCheck + WINCY)
		{
			m_fScrollY = m_fCheck + WINCY;
			m_eDir = DIR_NONE;
			return false;
		}
		return true;
	default:
		return false;
	}

}

