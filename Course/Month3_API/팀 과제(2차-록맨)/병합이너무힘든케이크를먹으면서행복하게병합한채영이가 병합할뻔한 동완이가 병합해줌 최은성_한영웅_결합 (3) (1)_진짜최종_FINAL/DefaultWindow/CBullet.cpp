#include "pch.h"
#include "CBullet.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CAnimMgr.h"
#include "CAbstractFactory.h"
CBullet::CBullet()
{
}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/player_weapon_all.bmp", L"Bullet");

	/*CAnimMgr::Get_Instance()->Insert_Animation(L"Bullet",
		CAbstractFactory<CAnimation>::Create(this,
			FPOINT(32.f, 30.f),
			80,
			3,
			L"Bullet"));*/


	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_fSpeed = 5.f;

	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;
}

int CBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));

	__super::Update_Rect();

	return OBJ_NOEVENT;
}
void CBullet::Late_Update()
{

}

void CBullet::Render(HDC hDC)
{


	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Bullet");
	/*CAnimMgr::Get_Instance()->Render(hDC, L"Bullet");*/
	//BitBlt(hDC,						// 복사 받을 DC
	//	m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
	//	m_tRect.top + iScrollY,// 복사 받을 위치 좌표 X, Y	
	//	(int)m_tInfo.fCX,		// 복사 받을 이미지의 가로, 세로
	//	(int)m_tInfo.fCY,
	//	hMemDC,						// 복사할 이미지 DC
	//	0,//레프트							// 비트맵 출력 시작 좌표(Left, top)
	//	0,
	//	SRCCOPY);					// 출력 효과 설정(그대로 출력)

	GdiTransparentBlt(
		hDC,                        // 복사 받을 DC (화면에 출력할 DC)
		m_tRect.left + iScrollX,    // 복사받을 위치의 X좌표 (현재 스크롤 값을 고려하여 계산)
		m_tRect.top + iScrollY,     // 복사받을 위치의 Y좌표 (현재 스크롤 값을 고려하여 계산)
		32,                         // 복사받을 이미지의 가로 길이 (출력할 이미지 크기 지정)
		31,                         // 복사받을 이미지의 세로 길이
		hMemDC,                     // 복사할 이미지가 저장된 메모리 DC
		473,                        // 원본 비트맵에서 복사를 시작할 X좌표
		616,                        // 원본 비트맵에서 복사를 시작할 Y좌표
		32,                         // 원본 비트맵에서 복사할 영역의 가로 길이
		31,                         // 원본 비트맵에서 복사할 영역의 세로 길이
		RGB(128, 0, 128));       // 투명 처리할 색상 (이미지의 이 색상이 투명 처리됨)
}


void CBullet::Release()
{
}

void CBullet::Default_Bullet()
{
	m_tInfo.fCX = 20.f; // 총알 크기
	m_tInfo.fCY = 20.f;
	m_fSpeed = 10.f;

	Check_Bul = Default;
}

void CBullet::Skill_F1_Bullet()
{

	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;

	m_fSpeed = 15.f;

	Check_Bul = Skill_1;

}

void CBullet::Skill_F2_Bullet()
{

	m_tInfo.fCX = 20.f; // 크기
	m_tInfo.fCY = 20.f;
	m_tRect.right = 50;
	m_tRect.left = 50;


	m_fSpeed = 20.f; //총알속도

	Check_Bul = Skill_2;

}

