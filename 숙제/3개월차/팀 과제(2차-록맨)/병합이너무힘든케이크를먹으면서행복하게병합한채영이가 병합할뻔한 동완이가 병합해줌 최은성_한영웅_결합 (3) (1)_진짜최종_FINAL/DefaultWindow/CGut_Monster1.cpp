#include "pch.h"
#include "CGut_Monster1.h"
#include "CObjMgr.h"
#include "CAnimMgr.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CAnimation.h"
#include "CGut_M1_Bullet.h"
#include "CCollisionMgr.h"

CGut_Monster1::CGut_Monster1() : m_fAttackRange(0.f)
{
	m_enState = STATE_IDLE;
	m_eDir = DIR_LEFT;

	Hp_Count = 0;
	MAX_Hp = 10;

	m_bJump = true;
	m_fTime = 0.f;
}

void CGut_Monster1::Initialize()
{
	m_tInfo = { 300, 200, 64, 52 };
	m_fAttackRange = 250.f;
	m_fJumpPower = 13.f;
	m_iStateCooldown = 30;
	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	Initialize_Animation();
}

void CGut_Monster1::Initialize_Animation()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Enemy_Left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all_flip.bmp", L"Enemy_Right");
	FPOINT t_Size = { m_tInfo.fCX, m_tInfo.fCY };

	ANIMATION_CREATE(L"gut1_idle_left", this, t_Size, 500, 1, L"Enemy_Left");
	ANIMATION_EDIT(L"gut1_idle_left", 284, 52, t_Size, 0);
	ANIMATION_CREATE(L"gut1_idle_right", this, t_Size, 500, 1, L"Enemy_Right");
	ANIMATION_EDIT(L"gut1_idle_right", 1032 - 284 - 64, 52, t_Size, 0);

	ANIMATION_CREATE(L"gut1_ready_left", this, t_Size, 500, 1, L"Enemy_Left");
	ANIMATION_EDIT(L"gut1_ready_left", 348, 52, t_Size, 0);
	ANIMATION_CREATE(L"gut1_ready_right", this, t_Size, 500, 1, L"Enemy_Right");
	ANIMATION_EDIT(L"gut1_ready_right", 1032 - 348 - 64, 52, t_Size, 0);

	ANIMATION_CREATE(L"gut1_attack_left", this, t_Size, 500, 1, L"Enemy_Left");
	ANIMATION_EDIT(L"gut1_attack_left", 412, 52, t_Size, 0);
	ANIMATION_CREATE(L"gut1_attack_right", this, t_Size, 500, 1, L"Enemy_Right");
	ANIMATION_EDIT(L"gut1_attack_right", 1032 - 412 - 64, 52, t_Size, 0);
}

int CGut_Monster1::Update()
{
	Jumping();
	if (m_bDead)
		return OBJ_DEAD;

	float Distance = CCollisionMgr::Collision_RangeChack(m_pTarget, this);
	if (Distance <= m_fAttackRange)
	{
		if (m_enState == STATE_IDLE)
			m_enState = STATE_READY;
	}
	else
		m_enState = STATE_IDLE;


	switch (m_enState)
	{
	case CGut_Monster1::STATE_IDLE:
		break;
	case CGut_Monster1::STATE_READY:
		//cooldown변수 다시 필요 가까워지면 ready로 변경 ready에서 cooldown지나면 attack으로 변경 attack도 cooldown 주고..
		m_iStateCooldown--;

		break;
	case CGut_Monster1::STATE_ATTACK:
		if (m_iStateCooldown == 30)
			CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CGut_M1_Bullet>::Create(m_tInfo.fX, m_tInfo.fY, this));
		m_iStateCooldown--;
		break;
	case CGut_Monster1::STATE_END:
		break;
	default:
		break;
	}
	if (m_iStateCooldown == 0)
	{
		if (m_enState == STATE_ATTACK)
		{
			m_enState = STATE_READY;
			m_iStateCooldown = 30;
		}
		else if (m_enState == STATE_READY)
		{
			m_iStateCooldown = 30;
			m_enState = STATE_ATTACK;
		}
		else
			m_iStateCooldown = 30;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGut_Monster1::Late_Update()
{
	if (CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX < m_tInfo.fX)
		m_eDir = DIR_LEFT;
	else
		m_eDir = DIR_RIGHT;
	
}

void CGut_Monster1::Render(HDC hDC)
{
	switch (m_enState)
	{
	case CGut_Monster1::STATE_IDLE:
		if (m_eDir == DIR_LEFT)
			ANIMATION_RENDER(hDC, L"gut1_idle_left", this);
		else
			ANIMATION_RENDER(hDC, L"gut1_idle_right", this);
		break;
	case CGut_Monster1::STATE_READY:
		if (m_eDir == DIR_LEFT)
			ANIMATION_RENDER(hDC, L"gut1_ready_left", this);
		else
			ANIMATION_RENDER(hDC, L"gut1_ready_right", this);
		break;
	case CGut_Monster1::STATE_ATTACK:
		if (m_eDir == DIR_LEFT)
			ANIMATION_RENDER(hDC, L"gut1_attack_left", this);
		else
			ANIMATION_RENDER(hDC, L"gut1_attack_right", this);
		break;
	default:
		break;
	}
}

void CGut_Monster1::Release()
{
}
