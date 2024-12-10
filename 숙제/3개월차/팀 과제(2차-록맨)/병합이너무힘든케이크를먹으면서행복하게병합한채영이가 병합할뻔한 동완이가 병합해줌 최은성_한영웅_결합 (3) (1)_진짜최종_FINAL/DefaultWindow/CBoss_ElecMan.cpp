#include "pch.h"
#include "CBoss_ElecMan.h"
#include "CBullet.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"
#include "CAbstractFactory.h"
#include "CBullet_Elec.h"
#include "CUIMgr.h"
#include "CBossHpBar.h"
#include "CScrollMgr.h"
#include "CEffect.h"

CBoss_ElecMan::CBoss_ElecMan()
	: m_p_Boss_Bullet(nullptr), m_Boss_pPlayer(nullptr), m_fAttackRange(0.f), m_ulTime(0), m_BossState(IDLE),
	m_fChaseRange(0), m_iCount(0)
{
	ZeroMemory(&m_tIdle, sizeof(IMAGE));
	ZeroMemory(&m_tAttack1, sizeof(IMAGE));
	ZeroMemory(&m_tAttack2, sizeof(IMAGE));
	ZeroMemory(&m_tNowState, sizeof(IMAGE));
	ZeroMemory(&m_tChase1, sizeof(IMAGE));
	ZeroMemory(&m_tChase2, sizeof(IMAGE));
	ZeroMemory(&m_tJump, sizeof(IMAGE));

}

CBoss_ElecMan::~CBoss_ElecMan()
{
}

void CBoss_ElecMan::Initialize()
{
	//임시
	m_tInfo = { 300, 450, 71, 71 };

	m_eBossType = BT_ELECMAN;

	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	m_p_Boss_Bullet = CObjMgr::Get_Instance()->Get_List(OBJ_BOSSBULLET);

	m_fAttackRange = 150.f;
	m_fChaseRange = 300.f;

	m_tIdle = { 0, 0 };
	m_tAttack2 = { 413, 0 };
	m_tAttack1 = { 486, 0 };
	m_tChase1 = { 280, 0 };
	m_tChase2 = { 347,0 };
	m_tJump = { 214, 0 };
	m_iHp = 80;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_elec_all.bmp", L"Boss_Elec");

	// 보스 UI
	CUIMgr::Get_Instance()->Add_UI(UI_HP_BOSS, CAbstractFactory<CBossHpBar>::CreateUI(this));
}

int CBoss_ElecMan::Update()
{
	if (m_bDead || m_iHp <= 0)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CEffect>::Create(m_tInfo.fX - 10.f, m_tInfo.fY - 5.f));
		return OBJ_DEAD;
	}


	float Distance = CCollisionMgr::Collision_RangeChack(m_pTarget, this);

	if (Distance <= m_fChaseRange && Distance > m_fAttackRange)
	{
		m_BossState = CHASE;
		if (m_tInfo.fX > m_pTarget->Get_Info().fX)
			m_tInfo.fX -= 3.f;
		if (m_tInfo.fX < m_pTarget->Get_Info().fX)
			m_tInfo.fX += 3.f;
	}
	else if (Distance <= m_fAttackRange)
	{
		m_BossState = ATTACK;
	}
	else
	{
		m_BossState = IDLE;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CBoss_ElecMan::Late_Update()
{
	switch (m_BossState)
	{
	case IDLE:
		m_tNowState = m_tIdle;
		break;
	case ATTACK:
	{
		if ((m_tNowState == m_tAttack1) ||
			(m_tNowState == m_tChase1) ||
			(m_tNowState == m_tChase2))
		{
			if (++m_iCount >= 40)
			{
				m_tNowState = m_tAttack2;
				m_iCount = 0;
			}
		}
		else if (m_tNowState == m_tAttack2)
		{
			if (++m_iCount >= 20)
			{
				m_p_Boss_Bullet->push_back(Create_Bullet(BUL_ELEC));
				m_tNowState = m_tAttack1;
				m_iCount = 0;
			}
		}
		break;
	}
	case CHASE:
	{
		if (m_tNowState == m_tChase1 ||
			m_tNowState == m_tIdle ||
			m_tNowState == m_tAttack1 ||
			m_tNowState == m_tAttack2)
		{
			if (++m_iCount >= 10)
			{
				m_tNowState = m_tChase2;
				m_iCount = 0;
			}
		}
		else if (m_tNowState == m_tChase2)
		{
			if (++m_iCount >= 10)
			{
				m_tNowState = m_tChase1;
				m_iCount = 0;
			}
		}
		break;
	}
	case JUMP:
	{
		m_tNowState = m_tJump;
	}
	default:
		break;
	}
}

void CBoss_ElecMan::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Elec");

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,						// 복사 받을 DC
		m_tInfo.fX + iScrollX,
		m_tInfo.fY + iScrollY, 					// 복사 받을 위치 좌표 X, Y			
		m_tInfo.fCX, m_tInfo.fCY,				// 복사 받을 이미지의 가로, 세로
		hMemDC,									// 복사할 이미지 DC	
		m_tNowState._iX, m_tNowState._iY,								// 비트맵 출력 시작 좌표(Left, top)
		71, 71,									// 복사할 이미지의 가로, 세로
		RGB(128, 0, 128));						// 출력 효과 설정(그대로 출력)
}

void CBoss_ElecMan::Release()
{
}

CObj* CBoss_ElecMan::Create_Bullet(BULLET_ID _type)
{
	if (m_pTarget->Get_Info().fX > m_tInfo.fX)
	{
		m_fAngle = 0.f;
	}
	else if (m_pTarget->Get_Info().fX <= m_tInfo.fX)
	{
		m_fAngle = 180.f;
	}

	return CAbstractFactory<CBullet_Elec>::Create(m_tInfo.fX, m_tInfo.fY, m_fAngle);
}
