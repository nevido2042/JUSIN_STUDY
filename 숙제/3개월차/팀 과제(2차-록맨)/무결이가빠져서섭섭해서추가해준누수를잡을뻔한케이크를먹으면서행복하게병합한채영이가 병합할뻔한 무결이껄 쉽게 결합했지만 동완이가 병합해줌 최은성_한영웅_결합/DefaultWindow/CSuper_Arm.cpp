#include "pch.h"
#include "CSuper_Arm.h"
#include "CScrollMgr.h"
#include "CAbstractFactory.h"
#include "Boss_GutsMan.h"
#include "CAnimMgr.h"
#include "CBmpMgr.h"
#include "CPlayer.h"

CSuper_Arm::CSuper_Arm()
{
}

CSuper_Arm::~CSuper_Arm()
{
	Release();
}

void CSuper_Arm::Initialize()
{
	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 64.f;
	m_tInfo.fX = m_pTarget->Get_Info().fX;
	m_tInfo.fY = m_pTarget->Get_Info().fY;


	m_fSpeed = 10.f;//이동속도
	m_fDistance = 100.f;//이동거리
	if (dynamic_cast<CPlayer*>(m_pTarget) != nullptr)
		m_fJumpPower = 10.f; //기본 점프 파워
	m_iHp = 1; // 플레이어 HP(은성)
	m_fTime = 0.f;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/super_block.bmp", L"Super_Block");
	CAnimMgr::Get_Instance()->Insert_Animation(L"Super_Block", CAbstractFactory<CAnimation>::Create(this, FPOINT(64.f, 64.f), 1, 1, L"Super_Block"));
	m_eDir = m_pTarget->Get_Direction();


}

int CSuper_Arm::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (dynamic_cast<CPlayer*>(m_pTarget) != nullptr)
		m_fJumpPower = 13.f; //기본 점프 파워

	if ((m_pTarget->Get_HoldBullet()))
	{
		if (m_eDir == DIR_RIGHT)
		{
			m_tInfo.fX = m_pTarget->Get_Info().fX - m_pTarget->Get_Info().fCX * 0.5f;
			m_tInfo.fY = m_pTarget->Get_Info().fY - m_pTarget->Get_Info().fCY;
		}
		else
		{
			m_tInfo.fX = m_pTarget->Get_Info().fX + m_pTarget->Get_Info().fCX * 0.5f;
			m_tInfo.fY = m_pTarget->Get_Info().fY - m_pTarget->Get_Info().fCY ;
		}
	}
	else
	{
		if (m_eDir == DIR_LEFT)
			m_tInfo.fX -= m_fJumpPower * cosf(45.f) * m_fTime;
		else if (m_eDir == DIR_RIGHT)
			m_tInfo.fX += m_fJumpPower * cosf(45.f) * m_fTime;
		m_tInfo.fY -= (m_fJumpPower * sinf(45.f) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
		m_fTime += 0.1f;

		if (m_tInfo.fY >= WINCY || m_tInfo.fX >= WINCX || m_tInfo.fX <=0)
		{
			m_fTime = 0.f;
			m_bDead = true;
		}
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CSuper_Arm::Late_Update()
{
}

void CSuper_Arm::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	CAnimMgr::Get_Instance()->Render(hDC, L"Super_Block", this);
}

void CSuper_Arm::Release()
{
}
