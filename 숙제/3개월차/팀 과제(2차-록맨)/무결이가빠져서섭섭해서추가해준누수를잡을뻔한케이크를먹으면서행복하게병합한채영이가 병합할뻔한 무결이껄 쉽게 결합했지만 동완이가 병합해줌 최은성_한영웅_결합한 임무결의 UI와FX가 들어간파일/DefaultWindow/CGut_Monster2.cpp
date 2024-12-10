#include "pch.h"
#include "CGut_Monster2.h"
#include "CObjMgr.h"
#include "CAnimMgr.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CAnimation.h"
#include "CGut_M2_Bullet.h"
#include "CCollisionMgr.h"

CGut_Monster2::CGut_Monster2() : m_fAttackRange(0.f)
{
	m_enState = STATE_IDLE;
	m_eDir = DIR_LEFT;

	Hp_Count = 0;
	m_iHp = 10;

	m_bJump = true;
	m_fTime = 0.f;

}

void CGut_Monster2::Initialize()
{
	m_tInfo = { 400, 536, 36, 30 };
	m_fAttackRange = 250.f;
	m_fJumpPower = 13.f;
	m_iStateCooldown = 30;
	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	Initialize_Animation();
}

void CGut_Monster2::Initialize_Animation()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Enemy_Left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all_flip.bmp", L"Enemy_Right");
	FPOINT t_Size = { 48, 30 };

	ANIMATION_CREATE(L"gut2_idle_left", this, t_Size, 500, 1, L"Enemy_Left");
	ANIMATION_EDIT(L"gut2_idle_left", 2, 74, t_Size, 0);
	ANIMATION_CREATE(L"gut2_idle_right", this, t_Size, 500, 1, L"Enemy_Right");
	ANIMATION_EDIT(L"gut2_idle_right", 1032 - 2 - 48, 74, t_Size, 0);

	ANIMATION_CREATE(L"gut2_attack_left", this, t_Size, 500, 1, L"Enemy_Left");
	ANIMATION_EDIT(L"gut2_attack_left", 50, 74, t_Size, 0);
	ANIMATION_CREATE(L"gut2_attack_right", this, t_Size, 500, 1, L"Enemy_Right");
	ANIMATION_EDIT(L"gut2_attack_right", 1032 - 50 - 48, 74, t_Size, 0);
}

int CGut_Monster2::Update()
{
	if (m_bDead || m_iHp <= 0)
		return OBJ_DEAD;

	float Distance = CCollisionMgr::Collision_RangeChack(m_pTarget, this);
	if (Distance <= m_fAttackRange)
	{
		if (m_enState == STATE_IDLE)
			m_enState = STATE_ATTACK;
	}
	else
		m_enState = STATE_IDLE;


	switch (m_enState)
	{
	case CGut_Monster2::STATE_IDLE:
		break;
		break;
	case CGut_Monster2::STATE_ATTACK:
		if (m_iStateCooldown == 30)
			CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CGut_M2_Bullet>::Create(m_tInfo.fX, m_tInfo.fY, this));
		m_iStateCooldown--;
		break;
	case CGut_Monster2::STATE_END:
		break;
	default:
		break;
	}
	if (m_iStateCooldown == 0)
	{
		m_iStateCooldown = 50;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGut_Monster2::Late_Update()
{
	if (CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX < m_tInfo.fX)
		m_eDir = DIR_LEFT;
	else
		m_eDir = DIR_RIGHT;


	
}

void CGut_Monster2::Render(HDC hDC)
{
	switch (m_enState)
	{
	case CGut_Monster2::STATE_IDLE:
		if (m_eDir == DIR_LEFT)
			ANIMATION_RENDER(hDC, L"gut2_idle_left", this);
		else
			ANIMATION_RENDER(hDC, L"gut2_idle_right", this);
		break;
	case CGut_Monster2::STATE_ATTACK:
		if (m_eDir == DIR_LEFT)
			ANIMATION_RENDER(hDC, L"gut2_attack_left", this);
		else
			ANIMATION_RENDER(hDC, L"gut2_attack_right", this);
		break;
	default:
		break;
	}
}

void CGut_Monster2::Release()
{
}
