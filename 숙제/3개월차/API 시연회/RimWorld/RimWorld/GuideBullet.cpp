#include "pch.h"
#include "GuideBullet.h"
#include "ObjMgr.h"

CGuideBullet::CGuideBullet()
{
}

CGuideBullet::~CGuideBullet()
{
	Release();
}

void CGuideBullet::Initialize()
{
	m_tInfo.fCX = 20.f;
	m_tInfo.fCY = 20.f;

	m_fSpeed = 5.f;
}

int CGuideBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pTarget = CObjMgr::Get_Instance()->Get_Target(OBJ_MONSTER, this);

	if (m_pTarget)
	{
		float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f);

		fWidth = m_pTarget->Get_Info().fX - m_tInfo.fX;
		fHeight = m_pTarget->Get_Info().fY - m_tInfo.fY;

		fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		fRadian = acosf(fWidth / fDiagonal);

		   if (m_pTarget->Get_Info().fY > m_tInfo.fY)
		       fRadian = (2.f * PI) - fRadian;		

		  m_fAngle = fRadian * (180.f / PI);
	}
	
	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));

	__super::Update_Rect();


	return OBJ_NOEVENT;
}

int CGuideBullet::Late_Update()
{
	return 0;
}

void CGuideBullet::Render(HDC hDC)
{
	Ellipse(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tRect.right,
		m_tRect.bottom);
}

void CGuideBullet::Release()
{
}
