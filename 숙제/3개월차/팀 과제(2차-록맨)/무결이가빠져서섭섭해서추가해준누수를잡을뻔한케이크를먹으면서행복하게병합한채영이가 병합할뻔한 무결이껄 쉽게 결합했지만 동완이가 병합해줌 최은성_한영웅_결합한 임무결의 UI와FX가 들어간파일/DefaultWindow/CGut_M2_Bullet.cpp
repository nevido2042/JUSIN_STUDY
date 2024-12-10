#include "pch.h"
#include "CGut_M2_Bullet.h"
#include "CScrollMgr.h"
#include "CAbstractFactory.h"
#include "Boss_GutsMan.h"
#include "CAnimMgr.h"
#include "CBmpMgr.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CObjMgr.h"

void CGut_M2_Bullet::Initialize()
{
	m_tInfo.fCX = 12.f;
	m_tInfo.fCY = 12.f;
	m_tInfo.fX = m_pTarget->Get_Info().fX;
	m_tInfo.fY = m_pTarget->Get_Info().fY;
	m_fSpeed = 5.f;//이동속도
	m_iHp = 10; // 플레이어 HP(은성)
	//m_eDir = CObjMgr::Get_Instance()->Get_Player
	m_eDir = m_pTarget->Get_Direction();
	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;
	m_fJumpPower = 13.f;
	Initialize_Animation();
}

void CGut_M2_Bullet::Initialize_Animation()
{
	FPOINT t_Size = { 12,12 };
	ANIMATION_CREATE(L"m2_bullet", this, t_Size, 50, 1, L"Enemy_Left");
	ANIMATION_EDIT(L"m2_bullet", 102, 82, t_Size, 0);
}

int CGut_M2_Bullet::Update()
{
	if (m_bDead || m_iHp <= 0)
		return OBJ_DEAD;

	m_tInfo.fX += m_fSpeed;


	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGut_M2_Bullet::Late_Update()
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	if (m_tInfo.fY >= WINCY + iScrollY || m_tInfo.fX >= WINCX + iScrollX || m_tInfo.fX <= 0 + iScrollX)
	{
		m_fTime = 0.f;
		m_bDead = true;
	}

}

void CGut_M2_Bullet::Render(HDC hDC)
{
	ANIMATION_RENDER(hDC, L"m2_bullet", this);
}

void CGut_M2_Bullet::Release()
{
}
