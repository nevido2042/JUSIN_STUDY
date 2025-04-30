#include "pch.h"
#include "CCannonBullet.h"

CCannonBullet::CCannonBullet()
	:m_fTime(0.f)
{

}

CCannonBullet::~CCannonBullet()
{
}

int CCannonBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f)) * m_fTime;
	m_tInfo.fY -= (m_fSpeed * sinf(m_fAngle * (PI / 180.f)) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
	m_fTime += 0.2f;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CCannonBullet::Initialize()
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_fSpeed = 20.f;
}
