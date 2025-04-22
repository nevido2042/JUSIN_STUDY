#include "pch.h"
#include "CFireWall.h"
#include "CScrollMgr.h"

void CFireWall::Initialize()
{
	m_tInfo = { 125, 532, 50, 150 };
	m_fOrigY = m_tInfo.fY;
	m_ullTime = GetTickCount64();
}

int CFireWall::Update()
{
	if (m_bFrozen)
		return OBJ_DEAD;
	if (!m_bIdle)
	{
		if (m_bMoveUp)
		{
			m_tInfo.fY -= 1.f;
			if (m_tInfo.fY <= m_fOrigY - 96.f)
			{
				m_bMoveUp = !m_bMoveUp;
				m_bIdle = !m_bIdle;
				m_tInfo.fY = m_fOrigY - 96.f;
				m_ullTime = GetTickCount64();
			}
		}
		else
		{
			m_tInfo.fY += 1.f;
			if (m_tInfo.fY >= m_fOrigY)
			{
				m_bMoveUp  = !m_bMoveUp;
				m_bIdle    = !m_bIdle;
				m_tInfo.fY = m_fOrigY;
				m_ullTime = GetTickCount64();
			}
		}
	}
	else
	{
		if (m_ullTime + 2000 < GetTickCount64())
		{
			m_bIdle = !m_bIdle;
		}
	}
	

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CFireWall::Late_Update()
{
	
}

void CFireWall::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	Rectangle(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top,
		m_tRect.right + iScrollX,
		m_tRect.bottom);
}

void CFireWall::Release()
{
}
