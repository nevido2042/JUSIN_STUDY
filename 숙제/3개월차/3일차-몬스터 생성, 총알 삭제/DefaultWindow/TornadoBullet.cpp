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
	m_fDistance = 50.f;
	m_fSpeed = 2.f;
}

int CTornadoBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fOrbitalAngle += 10.f;

	m_tInfo.fX = m_tFocalPoint.x + m_fDistance * cosf(m_fOrbitalAngle * (PI / 180.f));
	m_tInfo.fY = m_tFocalPoint.y - m_fDistance * sinf(m_fOrbitalAngle * (PI / 180.f));

	m_tFocalPoint.x += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	m_tFocalPoint.y -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));

	__super::Update_Rect();

	return OBJ_NOEVENT;
}
