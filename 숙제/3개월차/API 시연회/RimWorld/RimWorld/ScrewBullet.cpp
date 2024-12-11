#include "pch.h"
#include "ScrewBullet.h"

CScrewBullet::CScrewBullet()
	: m_bStart(false), m_fRotAngle(0.f), m_fRotSpeed(0.f)
{
	ZeroMemory(&m_tCenter, sizeof(POINT));
}

CScrewBullet::~CScrewBullet()
{
	Release();
}

void CScrewBullet::Initialize()
{
	m_tInfo.fCX = 20.f;
	m_tInfo.fCY = 20.f;

	m_fSpeed = 5.f;

	m_fRotAngle = 0.f;
	m_fRotSpeed = 30.f;
	m_fDistance = 20.f;

	m_bStart = true;
}

int CScrewBullet::Update()
{
	if (m_bStart)
	{
		m_tCenter.x = (long)m_tInfo.fX;
		m_tCenter.y = (long)m_tInfo.fY;
		m_bStart = false;
	}

	m_tCenter.x += long(m_fSpeed * cosf(m_fAngle * (PI / 180.f)));
	m_tCenter.y -= long(m_fSpeed * sinf(m_fAngle * (PI / 180.f)));

	m_fRotAngle += m_fRotSpeed;

	m_tInfo.fX = m_tCenter.x + (m_fDistance * cosf(m_fRotAngle * (PI / 180.f)));
	m_tInfo.fY = m_tCenter.y - (m_fDistance * sinf(m_fRotAngle * (PI / 180.f)));

	__super::Update_Rect();
	
	return OBJ_NOEVENT;
}

int CScrewBullet::Late_Update()
{
	return OBJ_NOEVENT;
}

void CScrewBullet::Render(HDC hDC)
{
	Ellipse(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tRect.right,
		m_tRect.bottom);
}

void CScrewBullet::Release()
{
}
