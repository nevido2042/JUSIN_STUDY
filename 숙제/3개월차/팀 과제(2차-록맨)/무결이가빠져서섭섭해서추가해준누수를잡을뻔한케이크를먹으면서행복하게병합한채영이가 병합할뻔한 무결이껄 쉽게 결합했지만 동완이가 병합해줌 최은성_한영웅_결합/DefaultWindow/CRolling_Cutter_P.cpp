#include "pch.h"
#include "CRolling_Cutter_P.h"
#include "CBmpMgr.h"

CRolling_Cutter_P::CRolling_Cutter_P() : m_fTime(),m_tStartPoint()
{
}

CRolling_Cutter_P::~CRolling_Cutter_P()
{
}

void CRolling_Cutter_P::Initialize()
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_fSpeed = 70.f;


	if (m_eDir == DIR_LEFT)
		m_fAngle = RADIAN(150.f);
	else
		m_fAngle = RADIAN(30.f);

	m_tStartPoint = Get_Point();

	m_ullTime = m_ullAnimationTime = GetTickCount64();
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_cut_all.bmp", L"CutMan");
}

int CRolling_Cutter_P::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_ullCerrentTime = GetTickCount64();



	if (m_ullTime + 750 < m_ullCerrentTime)
	{
		m_fSpeed = 10.f;
		Set_Angle(m_pCaster->Get_pInfo());
		m_tInfo.fX += m_fSpeed * cosf(m_fAngle);
		m_tInfo.fY -= m_fSpeed * sinf(m_fAngle);
	}
	else
	{
		m_fTime += 0.15f;
		m_tInfo.fX = m_tStartPoint.x + m_fSpeed * cosf(m_fAngle) * m_fTime;
		m_tInfo.fY = m_tStartPoint.y - (m_fSpeed * sinf(m_fAngle) * m_fTime - G_FORCE * SQUARE(m_fTime) * 0.5f);
	}

	if (m_ullAnimationTime + m_iAnimationFrame < m_ullCerrentTime)
	{
		Update_Animation();
		m_ullAnimationTime = m_ullCerrentTime;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CRolling_Cutter_P::Late_Update()
{
	if (m_ullTime + 1000 < m_ullCerrentTime)
	{
		RECT rc;
		if (IntersectRect(&rc, &m_tRect, m_pCaster->Get_Rect()))
		{
			m_bDead = true;
		}
	}
}