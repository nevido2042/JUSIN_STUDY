#include "pch.h"
#include "CBoss.h"
#include "CBullet.h"

//CBoss::CBoss()
//{
//
//	ZeroMemory(&m_p_Boss_Bullet, sizeof(m_p_Boss_Bullet));
//	ZeroMemory(&turretEnd, sizeof(turretEnd));
//	ZeroMemory(&HP_INFO, sizeof(HP_INFO));
//	m_Boss_pPlayer = NULL;
//	m_ullLast_Fire = 0;
//	m_fTurretAngle = 0.f; // 포신 각도 초기화
//	m_fTurretSpeed = 2.f; // 포신 회전 속도
//	Bullet_X = 40;
//	Hp_Count = 0;
//	MAX_Hp = 10;
//
//}
//
//CBoss::~CBoss()
//{
//}
//
//void CBoss::Initialize()
//{
//	m_tInfo = { 300, -200, 200.f, 200.f };
//	HP_INFO = { 200,200,200.f,200.f };
//	m_fSpeed = 3.f;
//}
//
//	}
//	if (GetAsyncKeyState('4'))  // 5개 유도탄
//	{
//		if (m_ullLast_Fire + 100 > GetTickCount64())
//		{
//			return;
//		}
//		m_ullLast_Fire = GetTickCount64();
//		m_p_Boss_Bullet->push_back(Create_Bullet(4));
//	}
//
//	if (m_ullLast_Fire + 800 > GetTickCount64())
//	{
//		return;
//	}
//	m_ullLast_Fire = GetTickCount64();
//	m_p_Boss_Bullet->push_back(Create_Bullet(1)); // 기본 총알
//
//
//}
//
//CObj* CBoss::Create_Bullet(int _type)
//{
//	CObj* pBullet = new CBullet;
//	if (_type == 1)
//	{
//		static_cast<CBullet*>(pBullet)->Default_Bullet();
//	}
//	else if (_type == 2)
//	{
//		static_cast<CBullet*>(pBullet)->Skill_F1_Bullet();
//	}
//	else if (_type == 3)
//	{
//		static_cast<CBullet*>(pBullet)->Skill_F2_Bullet();
//	}
//	pBullet->Set_Pos(m_tPosin.x, m_tPosin.y);
//	pBullet->Set_Ang(m_fAngle);
//	return pBullet;
//
//
//
//}
//





