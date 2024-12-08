#include "pch.h"
#include "Boss_IceMan.h"
#include "CScrollMgr.h"
#include "Ice_Slasher.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"

CBoss_IceMan::CBoss_IceMan()
	: /*m_fAccel(0.f),*/ m_fGravity(0.f), /*m_fFallSpeed(0.f),*/ m_bJump(false),
	m_fJumpSpeed(0.f), m_fTime(0.f), m_ullJumpTime(0), m_fPrevY(0.f),
	m_ullFireTime(0), m_pPlayer(nullptr)
{
}

CBoss_IceMan::~CBoss_IceMan()
{
	Release();
}

void CBoss_IceMan::Initialize()
{
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;
	m_fSpeed = 3.f;

	//m_fAccel = 0.1f;
	m_fGravity = 2.f;
	//m_fFallSpeed = m_fGravity;
	m_fJumpSpeed = 15.f;
	m_ullJumpTime = GetTickCount64();
	m_ullFireTime = GetTickCount64();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../megaman1 sprite/boss_ice_all.bmp", L"BossIceAll");

	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
}

int CBoss_IceMan::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Fall();

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBoss_IceMan::Late_Update()
{
	if (m_ullJumpTime + 100.f < GetTickCount64())
	{
		m_ullJumpTime = GetTickCount64();
		m_bJump = true;
	}

	if (m_ullFireTime + 500.f < GetTickCount64())
	{
		m_ullFireTime = GetTickCount64();
		Fire();
	}

	Jumping();
}

void CBoss_IceMan::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BossIceAll");

	GdiTransparentBlt(hDC,            // 복사 받을 DC
		m_tRect.left + iScrollX,    // 복사 받을 위치 좌표 X, Y    
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,            // 복사 받을 이미지의 가로, 세로    라이트 - 레프트 해서 길이를 넣어줘야하니까
		(int)m_tInfo.fCY,
		hMemDC,                        // 복사할 이미지 DC    
		7,                            // 비트맵 출력 시작 좌표(Left, top)
		14,
		(int)m_tInfo.fCX,            // 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(128, 0, 128));            // 제거할 색상
}

void CBoss_IceMan::Release()
{
}

void CBoss_IceMan::Fall()
{
	if (!m_bJump)
	{
		//m_fFallSpeed += m_fAccel;
		Set_PosY(m_fGravity);

		if (m_bGround)
		{
			m_fGravity;
		}
	}

}

void CBoss_IceMan::Jumping()
{
	if (m_bJump)
	{
		//플레이어의 위치를 확인해서 좌로 갈지 우로 갈지 판단
		if (m_pPlayer->Get_Info().fX < Get_Info().fX)
		{
			m_tInfo.fX -= cosf(45.f * PI / 180.f) * m_fTime;
		}
		else
		{
			m_tInfo.fX += cosf(45.f * PI / 180.f) * m_fTime;
		}

		
		m_tInfo.fY -= (m_fJumpSpeed * sinf(90.f) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
		m_fTime += 0.1f;

		//전 프레임의 높이와 현재 높이를 비교
		//떨어지고 있으면 점프 종료
		if (m_fPrevY < m_tInfo.fY)
		{
			m_bJump = false;
			m_fTime = 0.f;
		}

		m_fPrevY = m_tInfo.fY;
		Update_Rect();
	}
}

void CBoss_IceMan::Fire()
{
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CIce_Slasher>::Create(m_tInfo.fX, m_tInfo.fY, DIR_LEFT));
}
