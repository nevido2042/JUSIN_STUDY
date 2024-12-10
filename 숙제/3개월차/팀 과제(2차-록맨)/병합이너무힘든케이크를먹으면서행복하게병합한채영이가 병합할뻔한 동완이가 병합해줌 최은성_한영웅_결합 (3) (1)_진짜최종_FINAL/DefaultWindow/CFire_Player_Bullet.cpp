#include "pch.h"
#include "CBullet.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CAnimMgr.h"
#include "CAbstractFactory.h"
#include "CFire_Player_Bullet.h"
#include "CObjMgr.h"

CFire_Player_Bullet::CFire_Player_Bullet()
{
}

CFire_Player_Bullet::~CFire_Player_Bullet()
{
}


void CFire_Player_Bullet::Initialize()
{
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;
	m_fSpeed = 5.f;
	m_tInfo.fX = m_pPlayer->Get_Info().fX;
	m_tInfo.fY = m_pPlayer->Get_Info().fY;

	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/player_weapon_all.bmp", L"Player_Bullet");


}

int CFire_Player_Bullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += m_fSpeed;
	


	__super::Update_Rect();

	return 0;
}
void CFire_Player_Bullet::Late_Update()
{

}

void CFire_Player_Bullet::Render(HDC hDC)
{

	
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Bullet");
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




void CFire_Player_Bullet::Release()
{
}

