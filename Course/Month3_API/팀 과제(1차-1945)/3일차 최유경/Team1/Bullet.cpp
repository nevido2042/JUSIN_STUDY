#include "pch.h"
#include "Bullet.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
	Release();
}

void Bullet::Late_Update()
{
	if (m_tRect.left <= GAME_WIN_LEFT || m_tRect.top <= GAME_WIN_TOP || m_tRect.right >= GAME_WIN_RIGHT || m_tRect.bottom >= GAME_WIN_BOTTOM) {
		m_bDead = true;
	}

	if (m_iHp <= 0) {
		m_bDead = true;
	}
}

void Bullet::Render(HDC _hdc)
{
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(255, 255, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, myBrush);
	Ellipse(_hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	SelectObject(_hdc, oldBrush);
	DeleteObject(myBrush);
}

void Bullet::Release()
{
}
