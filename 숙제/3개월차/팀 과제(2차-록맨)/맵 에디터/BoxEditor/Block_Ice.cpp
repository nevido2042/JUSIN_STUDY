#include "pch.h"
#include "Block_Ice.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

void CBlock_Ice::Initialize()
{
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;
	
	m_eBlockType = BLOCK_ICE;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/tile_ice.bmp", L"Tile_Ice");
}

void CBlock_Ice::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Ice");

	BitBlt(hDC,						// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY ,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC
		100,							// 비트맵 출력 시작 좌표(Left, top)
		100,
		SRCCOPY);					// 출력 효과 설정(그대로 출력)

	//int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	/*Rectangle(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top,
		m_tRect.right + iScrollX,
		m_tRect.bottom);*/
}
