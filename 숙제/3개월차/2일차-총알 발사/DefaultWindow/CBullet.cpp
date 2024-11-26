#include "pch.h"
#include "CBullet.h"

CBullet::CBullet()
	:m_Direction(UP)
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

void CBullet::Update()
{
	switch (m_Direction)
	{
	case CBullet::UP:
		m_tInfo.fY -= m_fSpeed;
		break;
	case CBullet::LEFT:
		m_tInfo.fX -= m_fSpeed;
		break;
	case CBullet::DOWN:
		m_tInfo.fY += m_fSpeed;
		break;
	case CBullet::RIGHT:
		m_tInfo.fX += m_fSpeed;
		break;
	default:
		break;
	}


	__super::Update_Rect();
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
