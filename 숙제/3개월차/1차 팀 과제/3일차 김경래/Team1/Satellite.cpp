#include "pch.h"
#include "Satellite.h"

Satellite::Satellite()
{
}

Satellite::~Satellite()
{
	Release();
}

void Satellite::Initialize()
{
	m_tInfo.fCX = 15.f;
	m_tInfo.fCY = 15.f;
	m_fDistance = 30.f;
	m_fSpeed = 5.f;

	m_iHp = 10;
	m_iDamage = 10;
}

int Satellite::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fAngle += m_fSpeed;
	m_tInfo.fX = m_pTarget->Get_Info().fX + (m_fDistance * cosf(m_fAngle * (PI / 180.f)));
	m_tInfo.fY = m_pTarget->Get_Info().fY - (m_fDistance * sinf(m_fAngle * (PI / 180.f)));

	Obj::Update_Rect();
	return OBJ_NOEVENT;
}

void Satellite::Late_Update()
{
	if (m_iHp <= 0) {
		m_bDead = true;
	}
}

void Satellite::Render(HDC _hdc)
{
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(247, 230, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, myBrush);
	Ellipse(_hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	SelectObject(_hdc, oldBrush);
	DeleteObject(myBrush);
}

void Satellite::Release()
{
}
