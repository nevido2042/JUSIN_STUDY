#include "pch.h"
#include "CBullet.h"

CBullet::CBullet()
{
}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_fSpeed = 5.f;
}

int CBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	switch (m_eDir)
	{
	case DIR_LEFT:
		m_tInfo.fX -= m_fSpeed;
		break;

	case DIR_RIGHT:
		m_tInfo.fX += m_fSpeed;
		break;

	case DIR_UP:
		m_tInfo.fY -= m_fSpeed;
		break;

	case DIR_DOWN:
		m_tInfo.fY += m_fSpeed;
		break;

	case DIR_LU:
		m_tInfo.fY -= m_fSpeed;
		m_tInfo.fX -= m_fSpeed;
		break;

	case DIR_RU:
		m_tInfo.fY -= m_fSpeed;
		m_tInfo.fX += m_fSpeed;
		break;

	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}
void CBullet::Late_Update()
{
	if (100 >= m_tRect.left				||
		WINCX - 100 <= m_tRect.right	||
		100 >= m_tRect.top				||
		WINCY - 100 <= m_tRect.bottom)
	{
		m_bDead = true;
	}
}

void CBullet::Render(HDC hDC)
{
	Ellipse(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tRect.right,
		m_tRect.bottom);
}

void CBullet::Release()
{
}

