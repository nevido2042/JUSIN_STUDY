#include "pch.h"
#include "CGut_M1_Bullet.h"
#include "CScrollMgr.h"
#include "CAbstractFactory.h"
#include "Boss_GutsMan.h"
#include "CAnimMgr.h"
#include "CBmpMgr.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CObjMgr.h"

void CGut_M1_Bullet::Initialize()
{
	m_tInfo.fCX = 24.f;
	m_tInfo.fCY = 24.f;
	m_tInfo.fX = m_pTarget->Get_Info().fX;
	m_tInfo.fY = m_pTarget->Get_Info().fY;
	m_fSpeed = 5.f;//이동속도
	m_iHp = 1; // 플레이어 HP(은성)
	//m_eDir = CObjMgr::Get_Instance()->Get_Player
	m_eDir = m_pTarget->Get_Direction();
	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;
	m_fJumpPower = 13.f;
	Initialize_Animation();
}

void CGut_M1_Bullet::Initialize_Animation()
{
	FPOINT t_Size = { 22,24 };
	ANIMATION_CREATE(L"m1_bullet", this, t_Size, 50, 4, L"Enemy_Left");
	ANIMATION_EDIT(L"m1_bullet", 476, 66, t_Size, 0);
	ANIMATION_EDIT(L"m1_bullet", 542, 66, t_Size, 2);
	t_Size = { 24,22 };
	ANIMATION_EDIT(L"m1_bullet", 508, 66, t_Size, 1);
	ANIMATION_EDIT(L"m1_bullet", 574, 66, t_Size, 3);
}

int CGut_M1_Bullet::Update()
{
	if (m_bDead || m_iHp <= 0)
		return OBJ_DEAD;

	m_tInfo.fX += m_fSpeed;

	if (m_eDir == DIR_LEFT)
		m_tInfo.fX -= m_fJumpPower * cosf(88 * PI / 180) * m_fTime;
	else if (m_eDir == DIR_RIGHT)
		m_tInfo.fX += m_fJumpPower * cosf(88 * PI / 180) * m_fTime;
	m_tInfo.fY -= (m_fJumpPower * sinf(88 * PI / 180) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
	m_fTime += 0.1f;


	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGut_M1_Bullet::Late_Update()
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	if (m_tInfo.fY >= WINCY + iScrollY || m_tInfo.fX >= WINCX + iScrollX || m_tInfo.fX <= 0 + iScrollX)
	{
		m_fTime = 0.f;
		m_bDead = true;
	}
}

void CGut_M1_Bullet::Render(HDC hDC)
{
	ANIMATION_RENDER(hDC, L"m1_bullet", this);
}

void CGut_M1_Bullet::Release()
{
}
