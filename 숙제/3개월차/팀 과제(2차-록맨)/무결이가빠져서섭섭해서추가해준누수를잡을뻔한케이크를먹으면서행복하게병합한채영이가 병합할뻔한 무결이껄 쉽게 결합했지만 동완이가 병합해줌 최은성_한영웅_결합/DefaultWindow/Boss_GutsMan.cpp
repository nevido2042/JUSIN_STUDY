#include "pch.h"
#include "Boss_GutsMan.h"
#include "CBullet.h"
#include "CScrollMgr.h"
#include "CLineMgr.h"
#include "BlockMgr.h"
#include "CAbstractFactory.h"
#include "CSuper_Arm.h"
#include "CObjMgr.h"
#include "CAnimMgr.h"
#include "CPlayer.h"
// bullet이 key input 갖고
// key input이 입력되면 발사?
// 보스는?
// 겟 타겟 홀딩?

CBoss_GutsMan::CBoss_GutsMan()
{
	ZeroMemory(&HP_INFO, sizeof(HP_INFO));

	Bullet_X = 40;

	Hp_Count = 0;
	MAX_Hp = 10;

	m_bJump = true;
	m_fJumpPower = 13.f;
	m_fTime = 0.f;
	m_enState = STATE_IDLE;
	m_bHoldBullet = false;
	m_iStateCooldown = 30;
	m_eDir = DIR_LEFT;
	m_enMovingDir = DIR_LEFT;
}

void CBoss_GutsMan::Initialize()
{
	m_tInfo = { 500, 550, 62.f, 64.f };
	HP_INFO = { 200,200,200.f,200.f };
	m_fSpeed = 1.f;
	Initialize_Animation();
}

void CBoss_GutsMan::Initialize_Animation()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_gut_all.bmp", L"Gut_Left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_gut_all_flip.bmp", L"Gut_Right");
	FPOINT t_Size = { 62,64 };

	ANIMATION_CREATE(L"gut_idle_left", this, t_Size, 500, 1, L"Gut_Left");
	ANIMATION_CREATE(L"gut_hold_left", this, t_Size, 500, 1, L"Gut_Left");
	ANIMATION_CREATE(L"gut_attack_left", this, t_Size, 500, 1, L"Gut_Left");
	ANIMATION_CREATE(L"gut_jump_left", this, t_Size, 500, 1, L"Gut_Left");

	ANIMATION_CREATE(L"gut_idle_right", this, t_Size, 500, 1, L"Gut_Right");
	ANIMATION_CREATE(L"gut_hold_right", this, t_Size, 500, 1, L"Gut_Right");
	ANIMATION_CREATE(L"gut_attack_right", this, t_Size, 500, 1, L"Gut_Right");
	ANIMATION_CREATE(L"gut_jump_right", this, t_Size, 500, 1, L"Gut_Right");

	

}

int CBoss_GutsMan::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	switch (m_enState)
	{
	case CBoss_GutsMan::STATE_IDLE:
		m_iStateCooldown--;
		break;
	case CBoss_GutsMan::STATE_MOVE:
		if (m_enMovingDir == DIR_RIGHT) { m_tInfo.fX += 1.f; }
		else if (m_enMovingDir == DIR_LEFT) { m_tInfo.fX -= 1.f; }
		m_iStateCooldown--;
		break;
	case CBoss_GutsMan::STATE_HOLD:
		if (m_bHoldBullet == false)
		{
			m_bHoldBullet = true;
			CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CSuper_Arm>::Create(this));
		}
		m_iStateCooldown--;
		break;
	case CBoss_GutsMan::STATE_ATTACK:
		if (m_bHoldBullet == true)
			m_bHoldBullet = false;
		m_iStateCooldown--;
		break;
	case CBoss_GutsMan::STATE_JUMP:
		Jump();
		m_iStateCooldown--;
		break;
	default:
		break;
	}
	if (m_iStateCooldown == 0)
	{
		m_enState = GUTSTATE((m_enState + 1) % (int)STATE_END);
		switch (m_enState)
		{
		case CBoss_GutsMan::STATE_IDLE:
			m_iStateCooldown = 100;
			break;
		case CBoss_GutsMan::STATE_MOVE:
			m_iStateCooldown = 80;
			break;
		case CBoss_GutsMan::STATE_HOLD:
			m_iStateCooldown = 36;
			break;
		case CBoss_GutsMan::STATE_ATTACK:
			m_iStateCooldown = 100;
			break;
		case CBoss_GutsMan::STATE_JUMP:
			m_iStateCooldown = 500;
			break;
		case CBoss_GutsMan::STATE_END:
			break;
		default:
			break;
		}
		m_enMovingDir = DIRECTION(rand() % 2 + 1);
	}
	if (m_tInfo.fX >= WINCX - 100)
		m_tInfo.fX = WINCX - 100;
	else if (m_tInfo.fX <= 200)
		m_tInfo.fX = 200;
	if (m_tInfo.fY >= 526)
		m_tInfo.fY = 526;
	Update_Rect();
	return OBJ_NOEVENT;
}


void CBoss_GutsMan::Late_Update()
{
	if (CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX < m_tInfo.fX)
		m_eDir = DIR_LEFT;
	else
		m_eDir = DIR_RIGHT;
}


void CBoss_GutsMan::Render(HDC hDC)
{
	// 스크롤 위치 계산
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	// 보스 본체 사각형 렌더링
	Rectangle(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		m_tRect.right + iScrollX,
		m_tRect.bottom + iScrollY);

	// 체력 바 출력 (m_fSpeed 조건 확인)
	if (m_fSpeed == 0)
	{
		// 전체 체력 바 , 레프트,탑,라이트,바텀
		Rectangle(hDC, 90, 80, 110, 200);  // 일단 최대채력 580

		// 체력 바 색상 설정
		HBRUSH MyBrush = CreateSolidBrush(RGB(255, 0, 0));
		HBRUSH OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);

		// 현재 체력에 따른 바 길이 계산
		Rectangle(hDC, 90, 80, 110, (200 - Hp_Count));

		// 브러시 해제
		SelectObject(hDC, OldBrush);
		DeleteObject(MyBrush);
	}
}




void CBoss_GutsMan::Release()
{
}


CObj* CBoss_GutsMan::Create_Bullet()
{
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CSuper_Arm>::Create(this));

	return nullptr;
}



void CBoss_GutsMan::Jump()
{
	m_tInfo.fY -= (m_fJumpPower * sinf(45.f) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
	m_fTime += 0.03f;

	if (m_tInfo.fY >= 526 )
	{
		m_tInfo.fY = 526;
		m_fTime = 0.f;
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetJump() == false)
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_Fallen(m_eDir == DIR_LEFT?DIR_RIGHT:DIR_LEFT);
		m_iStateCooldown = 1;
	}
}
//





