#include "pch.h"
#include "CRolling_Cutter.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CBoss_CutMan.h"

CRolling_Cutter::CRolling_Cutter(): m_iImagePose(0), m_iAnimationFrame(80), m_ullCerrentTime(), m_bCurve(false), m_pCaster(nullptr)
{
}

CRolling_Cutter::~CRolling_Cutter()
{
}

void CRolling_Cutter::Initialize()
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_fSpeed = 6.f;

	//if (m_eDir == DIR_LEFT)
	//	m_fSpeed *= -1;

	Set_Angle(m_pTarget->Get_pInfo());
	m_ullTime = m_ullAnimationTime = GetTickCount64();
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_cut_all.bmp", L"CutMan");
}

int CRolling_Cutter::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_ullCerrentTime = GetTickCount64();

	m_tInfo.fX += m_fSpeed * cosf(m_fAngle);
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle);

	if (m_ullTime + 1500 < m_ullCerrentTime)
		Set_Angle(m_pCaster->Get_pInfo());

	if (m_ullAnimationTime + m_iAnimationFrame < m_ullCerrentTime)
	{
		Update_Animation();
		m_ullAnimationTime = m_ullCerrentTime;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CRolling_Cutter::Late_Update()
{
	if (m_ullTime + 1500 < m_ullCerrentTime)
	{
		RECT rc;
		if (IntersectRect(&rc, &m_tRect, m_pCaster->Get_Rect()))
		{
			m_bDead = true;
			static_cast<CBoss_CutMan*>(m_pCaster)->Set_Shoot();
		}
	}
}

void CRolling_Cutter::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CutMan");

	GdiTransparentBlt(hDC,            // 복사 받을 DC
		m_tRect.left + iScrollX,    // 복사 받을 위치 좌표 X, Y    
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,            // 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,                        // 복사할 이미지 DC    
		m_iAnimationX,                            // 비트맵 출력 시작 좌표(Left, top)
		m_iAnimationY,
		(int)m_tInfo.fCX,            // 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(128, 0, 128));
}

void CRolling_Cutter::Release()
{
}

void CRolling_Cutter::Set_Angle(INFO* _pTarget)
{
	float fDistance = (float)sqrt(SQUARE(m_tInfo.fX - _pTarget->fX) + SQUARE(m_tInfo.fY - _pTarget->fY));

	m_fAngle = acosf((_pTarget->fX - m_tInfo.fX) / fDistance);

	if (_pTarget->fY > m_tInfo.fY)
	{
		m_fAngle = (2.f * PI) - m_fAngle;
	}
}

void CRolling_Cutter::Update_Animation()
{
	m_iImagePose++;

	//m_iImagePose = 2;

	switch (m_iImagePose%4)
	{
	case 0:
		m_iAnimationX = 390;
		m_iAnimationY = 119;
		break;
	case 1:
		m_iAnimationX = 429;
		m_iAnimationY = 116;
		break;
	case 2:
		m_iAnimationX = 470;
		m_iAnimationY = 119;
		break;
	case 3:
		m_iAnimationX = 510;
		m_iAnimationY = 116;
		break;
	default:
		break;
	}
}
