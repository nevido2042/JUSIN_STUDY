#include "pch.h"
#include "CSilver_Mob1.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CSilver_Mob1::CSilver_Mob1() : m_ullTime(), m_iImagePose()
{
}

CSilver_Mob1::~CSilver_Mob1()
{
}

void CSilver_Mob1::Initialize()
{
	m_tInfo.fCX = 32.f;
	m_tInfo.fCY = 40.f;
	m_iAnimationX = 204;
	m_iAnimationY = 64;

	m_iHp = 10;

	m_fSpeed = 3.f;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Enemy");
}

int CSilver_Mob1::Update()
{
	if (m_bDead)
		return OBJ_DEAD;



	if (m_bDead || m_iHp <= 0)
		return OBJ_DEAD;

	m_ullCerrentTime = GetTickCount64();

	Set_Angle(m_pTarget->Get_pInfo());
	m_tInfo.fX += m_fSpeed * cosf(m_fAngle);
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle);

	if(m_ullTime + 50 < GetTickCount64())
		Update_Animation();

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CSilver_Mob1::Late_Update()
{
}

void CSilver_Mob1::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Enemy");

	GdiTransparentBlt(hDC,            // 복사 받을 DC
		m_tRect.left + iScrollX,    // 복사 받을 위치 좌표 X, Y    
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,            // 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,                        // 복사할 이미지 DC    
		m_iAnimationX,                            // 비트맵 출력 시작 좌표(Left, top)
		m_iAnimationY,
		(int)m_tInfo.fCX,            // 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(128, 0, 128));
}

void CSilver_Mob1::Release()
{
}

void CSilver_Mob1::Update_Animation()
{
	m_iImagePose++;

	switch (m_iImagePose % 2)
	{
	case 0:
		m_iAnimationX = 204;
		break;
	case 1:
		m_iAnimationX = 242;
		break;
	default:
		break;
	}
}

void CSilver_Mob1::Set_Angle(INFO* _pTarget)
{
	float fDistance = (float)sqrt(SQUARE(m_tInfo.fX - _pTarget->fX) + SQUARE(m_tInfo.fY - _pTarget->fY));

	m_fAngle = acosf((_pTarget->fX - m_tInfo.fX) / fDistance);

	if (_pTarget->fY > m_tInfo.fY)
	{
		m_fAngle = (2.f * PI) - m_fAngle;
	}
}
