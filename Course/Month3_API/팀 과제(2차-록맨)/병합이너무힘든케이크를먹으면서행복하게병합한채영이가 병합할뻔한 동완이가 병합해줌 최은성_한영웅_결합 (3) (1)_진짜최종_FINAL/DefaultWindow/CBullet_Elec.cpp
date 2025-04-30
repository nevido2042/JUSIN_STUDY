#include "pch.h"
#include "CBullet_Elec.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"


CBullet_Elec::CBullet_Elec() :iBulletCount(0)
{
	ZeroMemory(&m_tEelc_Bullet1, sizeof(IMAGE));
	ZeroMemory(&m_tEelc_Bullet2, sizeof(IMAGE));
	ZeroMemory(&m_tEelc_Bullet3, sizeof(IMAGE));
	ZeroMemory(&m_tEelc_Bullet4, sizeof(IMAGE));
	ZeroMemory(&m_tEelc_Bullet5, sizeof(IMAGE));
	ZeroMemory(&m_tEelc_Bullet6, sizeof(IMAGE));
	ZeroMemory(&m_tEelc_Bullet7, sizeof(IMAGE));
}

CBullet_Elec::~CBullet_Elec()
{
}

void CBullet_Elec::Initialize()
{
	m_eBulletId = BUL_ELEC;

	m_tInfo.fCX = 77.f;
	m_tInfo.fCY = 80.f;
	m_fSpeed = 5.f;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_elec_all.bmp", L"Boss_Elec");

	m_tEelc_Bullet1 = { 5,90 };
	m_tEelc_Bullet2 = { 89,90 };
	m_tEelc_Bullet3 = { 181,89 };
	m_tEelc_Bullet4 = { 262,90 };
	m_tEelc_Bullet5 = { 338,90 };
	m_tEelc_Bullet6 = { 422,92 };
	m_tEelc_Bullet7 = { 487,90 };

}

int CBullet_Elec::Update()
{
	if (m_bDead)
		return OBJ_DEAD;



	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBullet_Elec::Late_Update()
{
	iBulletCount++;

	if (iBulletCount < 5)
	{
		m_tNowState = m_tEelc_Bullet1;
	}
	else if (iBulletCount < 10)
	{
		m_tNowState = m_tEelc_Bullet2;
	}
	else if (iBulletCount < 15)
	{
		m_tNowState = m_tEelc_Bullet3;
	}
	else if (iBulletCount < 20)
	{
		m_tNowState = m_tEelc_Bullet4;
	}
	else if (iBulletCount < 25)
	{
		m_tNowState = m_tEelc_Bullet5;
	}
	else if (iBulletCount < 30)
	{
		m_tNowState = m_tEelc_Bullet6;
	}
	else if (iBulletCount < 40)
	{
		m_tNowState = m_tEelc_Bullet7;
		iBulletCount = 0;
	}
}

void CBullet_Elec::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Elec");

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,						// 복사 받을 DC
		m_tInfo.fX + iScrollX,
		m_tInfo.fY + iScrollY,					// 복사 받을 위치 좌표 X, Y			
		m_tInfo.fCX, m_tInfo.fCY,				// 복사 받을 이미지의 가로, 세로
		hMemDC,									// 복사할 이미지 DC	
		m_tNowState._iX, m_tNowState._iY,		// 비트맵 출력 시작 좌표(Left, top)
		m_tInfo.fCX, m_tInfo.fCY,				/// 복사할 이미지의 가로, 세로
		RGB(128, 0, 128));						// 출력 효과 설정(그대로 출력)
}

void CBullet_Elec::Release()
{

}