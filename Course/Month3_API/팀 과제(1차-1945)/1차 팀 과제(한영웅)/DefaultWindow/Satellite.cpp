#include "pch.h"
#include "Satellite.h"

CSatellite::CSatellite()
	:m_pPlayer(nullptr), m_fDistance(0.f), m_fAngle(0.f)
{
}

CSatellite::~CSatellite()
{
}

void CSatellite::Initialize()
{
	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 10.f;

	m_fSpeed = 5.f;

	m_fDistance = 50.f;
}

int CSatellite::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fAngle += m_fSpeed;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CSatellite::Late_Update()
{
	long lX  = long(m_pPlayer->Get_Info().fX + (m_fDistance * cosf(m_fAngle * (PI / 180.f))));
	long lY = long(m_pPlayer->Get_Info().fY - (m_fDistance * sinf(m_fAngle * (PI / 180.f))));
	Set_Pos(float(lX), float(lY));
}

void CSatellite::Render(HDC hDC)
{
	Ellipse(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tRect.right,
		m_tRect.bottom);
}

void CSatellite::Release()
{
}
