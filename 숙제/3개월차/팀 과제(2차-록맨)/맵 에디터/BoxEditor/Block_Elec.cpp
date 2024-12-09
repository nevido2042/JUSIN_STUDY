#include "pch.h"
#include "Block_Elec.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

void CBlock_Elec::Initialize()
{
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	m_eBlockType = BLOCK_ELEC;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/tile_elec.bmp", L"Tile_Elec");
}

void CBlock_Elec::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Elec");

	BitBlt(hDC,						// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC
		206,							// 비트맵 출력 시작 좌표(Left, top)
		3,
		SRCCOPY);					// 출력 효과 설정(그대로 출력)

}