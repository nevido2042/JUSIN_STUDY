#include "pch.h"
#include "CBullet_Normal.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CBullet_Normal::CBullet_Normal()
{
}

CBullet_Normal::~CBullet_Normal()
{
	Release();
}

void CBullet_Normal::Initialize()
{
	m_tInfo.fCX = 17.f;
	m_tInfo.fCY = 13.f;

	m_fSpeed = 5.f;

	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/player_weapon_all.bmp", L"Player_Bullet");
}

int CBullet_Normal::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += m_fSpeed;

	__super::Update_Rect();

	return 0;
}

void CBullet_Normal::Late_Update()
{
}

void CBullet_Normal::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Bullet");
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
		m_tInfo.fCX,                         // 복사받을 이미지의 가로 길이 (출력할 이미지 크기 지정)
		m_tInfo.fCY,                         // 복사받을 이미지의 세로 길이
		hMemDC,                     // 복사할 이미지가 저장된 메모리 DC
		507,                        // 원본 비트맵에서 복사를 시작할 X좌표
		25,                        // 원본 비트맵에서 복사를 시작할 Y좌표
		m_tInfo.fCX,                         // 원본 비트맵에서 복사할 영역의 가로 길이
		m_tInfo.fCY,                         // 원본 비트맵에서 복사할 영역의 세로 길이
		RGB(128, 0, 128));       // 투명 처리할 색상 (이미지의 이 색상이 투명 처리됨)
}

void CBullet_Normal::Release()
{
}
