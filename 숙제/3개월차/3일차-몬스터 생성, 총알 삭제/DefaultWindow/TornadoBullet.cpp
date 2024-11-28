#include "pch.h"
#include "TornadoBullet.h"

CTornadoBullet::CTornadoBullet()
	:m_fOrbitalAngle(0.f), m_fDistance(0.f)
{
	ZeroMemory(&m_tFocalPoint, sizeof(POINT));
}

CTornadoBullet::~CTornadoBullet()
{
}

void CTornadoBullet::Initialize()
{
	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 10.f;
	m_fDistance = 0.f;
	m_fSpeed = 1.5f;
}

int CTornadoBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fDistance += 0.2f;
	m_fOrbitalAngle += 10.f;

	//auto x = m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	//auto y = m_fSpeed * sinf(m_fAngle * (PI / 180.f));

	m_tFocalPoint.x += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	m_tFocalPoint.y -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CTornadoBullet::Late_Update()
{
	if (GAME_WIN_LEFT >= m_tRect.left ||
		GAME_WIN_RIGHT <= m_tRect.right ||
		GAME_WIN_TOP >= m_tRect.top ||
		GAME_WIN_BOTTOM <= m_tRect.bottom)
	{
		m_bDead = true;
	}

	m_tInfo.fX = m_tFocalPoint.x + m_fDistance * cosf(m_fOrbitalAngle * (PI / 180.f));
	m_tInfo.fY = m_tFocalPoint.y - m_fDistance * sinf(m_fOrbitalAngle * (PI / 180.f));

}

void CTornadoBullet::Render(HDC hDC)
{
	/*Ellipse(hDC,
		m_tFocalPoint.x - 1,
		m_tFocalPoint.y - 1,
		m_tFocalPoint.x + 1,
		m_tFocalPoint.y + 1);*/

	Ellipse(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tRect.right,
		m_tRect.bottom);
}
