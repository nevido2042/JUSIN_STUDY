#include "pch.h"
#include "ScrewBullet.h"

ScrewBullet::ScrewBullet()
	:m_fRotSpeed(0.f), m_fRotAngle(0.f), m_bStart(false)
{
	ZeroMemory(&m_tCenter, sizeof(POINT));

}

ScrewBullet::ScrewBullet(float _fAngle)
	:m_fRotSpeed(0.f), m_fRotAngle(0.f), m_bStart(false)
{
	m_fAngle = _fAngle;
	ZeroMemory(&m_tCenter, sizeof(POINT));
}

ScrewBullet::~ScrewBullet()
{
}

void ScrewBullet::Initialize()
{
	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 10.f;
	m_fSpeed = 5.f;
	m_iHp = 1;
	m_iDamage = 1;
	
	m_fDistance = 10.f;
	m_fRotSpeed = 30.f;

	m_bStart = true;
}

int ScrewBullet::Update()
{
	if (m_bDead) {
		return OBJ_DEAD;
	}

	if (m_bStart)
	{
		m_tCenter.x = (long)m_tInfo.fX;
		m_tCenter.y = (long)m_tInfo.fY;
		m_bStart = false;
	}

	m_tCenter.x = long(m_tCenter.x+ (m_fSpeed * cosf(m_fAngle * (PI / 180.f))));
	m_tCenter.y = long(m_tCenter.y-(m_fSpeed * sinf(m_fAngle * (PI / 180.f))));

	m_fRotAngle += m_fRotSpeed;

	m_tInfo.fX = long(m_tCenter.x + (m_fDistance * cosf(m_fRotAngle * (PI / 180.f))));
	m_tInfo.fY = long(m_tCenter.y - (m_fDistance * sinf(m_fRotAngle * (PI / 180.f))));



	Obj::Update_Rect();
	return OBJ_NOEVENT;
}

void ScrewBullet::Late_Update()
{
	if (m_tRect.left <= GAME_WIN_LEFT || m_tRect.top <= GAME_WIN_TOP || m_tRect.right >= GAME_WIN_RIGHT || m_tRect.bottom >= GAME_WIN_BOTTOM) {
		m_bDead = true;
	}

	if (m_iHp <= 0) {
		m_bDead = true;
	}
}

void ScrewBullet::Render(HDC _hdc)
{
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(220, 20, 60));
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, myBrush);
	Ellipse(_hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	SelectObject(_hdc, oldBrush);
	DeleteObject(myBrush);

	Ellipse(_hdc, m_tCenter.x - 1 , m_tCenter.y - 1, m_tCenter.x + 1, m_tCenter.y + 1);
}
