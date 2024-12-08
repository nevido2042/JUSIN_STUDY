#include "pch.h"
#include "Penguin.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

void CPenguin::Initialize()
{
    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 50.f;
    m_fSpeed = 3.f;

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../megaman1 sprite/enemy_all.bmp", L"EnemyAll");
}

void CPenguin::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"EnemyAll");

	GdiTransparentBlt(hDC,            // 복사 받을 DC
		m_tRect.left + iScrollX,    // 복사 받을 위치 좌표 X, Y    
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,            // 복사 받을 이미지의 가로, 세로    라이트 - 레프트 해서 길이를 넣어줘야하니까
		(int)m_tInfo.fCY,
		hMemDC,                        // 복사할 이미지 DC    
		215,							// 비트맵 출력 시작 좌표(Left, top)
		349,                           
		(int)m_tInfo.fCX,            // 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(128, 0, 128));
}
