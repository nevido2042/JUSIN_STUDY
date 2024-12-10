#include "pch.h"
#include "CSilver_Mob2.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CSilver_Mob2::CSilver_Mob2() : m_tStartPoint(), m_iInterY()
{
}

CSilver_Mob2::~CSilver_Mob2()
{
}

void CSilver_Mob2::Initialize()
{
	m_tInfo.fCX = 28.f;
	m_tInfo.fCY = 20.f;

	m_iAnimationX = 64;
	m_iAnimationY = 240;

	m_fAngle = RADIAN(115.f);
	m_tStartPoint = Get_Point();
	m_fJumpPower = 50.f;
	//106,221,134,260

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Enemy");

	m_ullTime = m_ullCerrentTime = GetTickCount64();
}

int CSilver_Mob2::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_ullCerrentTime = GetTickCount64();

	if (m_bJump == false && m_ullTime + 2000 < m_ullCerrentTime)
	{
		m_bJump = true;
		m_fJumpPower = 50.f;
		m_fTime = 0;
		m_tStartPoint.x = (long)m_tInfo.fX;
		m_tStartPoint.y = (long)m_tInfo.fY;
		m_ullTime = m_ullCerrentTime;
	}

	Jumping();
	Update_Animation();

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CSilver_Mob2::Late_Update() // 땅충돌시에 수행되어야할 코드인데, 일단 여기 박아놨음
{
}

void CSilver_Mob2::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Enemy");

	GdiTransparentBlt(hDC,            // 복사 받을 DC
		m_tRect.left + iScrollX,    // 복사 받을 위치 좌표 X, Y    
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,            // 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY + m_iInterY,
		hMemDC,                        // 복사할 이미지 DC    
		m_iAnimationX,                            // 비트맵 출력 시작 좌표(Left, top)
		m_iAnimationY,
		(int)m_tInfo.fCX,            // 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY + m_iInterY,
		RGB(128, 0, 128));



}

void CSilver_Mob2::Release()
{
}

void CSilver_Mob2::Jump_Pattern()
{
	m_tInfo.fX = m_tStartPoint.x + m_fJumpPower * cosf(m_fAngle) * m_fTime;
	m_tInfo.fY = m_tStartPoint.y - (m_fJumpPower * sinf(m_fAngle) * m_fTime - G_FORCE * SQUARE(m_fTime) * 0.5f);
	m_fTime += 0.15f;
}

void CSilver_Mob2::Update_Animation()
{
	if (m_bJump)
	{
		m_iInterY = 18.f;
		m_iAnimationX = 106;
		m_iAnimationY = 221;
	}
	else
	{
		m_iInterY = 0.f;
		m_iAnimationX = 64;
		m_iAnimationY = 240;
	}
}

void CSilver_Mob2::Jumping()
{
	if (m_bJump)
	{
		Jump_Pattern();
		if (m_fCellingY != 0 && (m_fCellingY > m_tInfo.fY))
		{
			m_tInfo.fY = m_fCellingY;
			//m_fJumpPower = 0.f;
			//m_fTime *= 0.5f;
		}

		if (m_fLineY != 0 && (m_fLineY < m_tInfo.fY))
		{
			m_bJump = false;
			m_fTime = 0.f;
			m_tInfo.fY = m_fLineY;
		}
	}
	else if (m_fLineY == 0.f || m_fLineY > m_tInfo.fY)
	{
		m_bJump = true;
		m_fJumpPower = 0.f;
	}
	else
	{
		m_tInfo.fY = m_fLineY;
	}
}