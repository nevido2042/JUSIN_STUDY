#include "pch.h"
#include "Block_Gut.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

void CBlock_Gut::Initialize()
{
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	m_eBlockType = BLOCK_GUT;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/tile_gut.bmp", L"Tile_Gut");
}

void CBlock_Gut::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Gut");

	BitBlt(hDC,						// ���� ���� DC
		m_tRect.left + iScrollX,	// ���� ���� ��ġ ��ǥ X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// ���� ���� �̹����� ����, ����
		(int)m_tInfo.fCY,
		hMemDC,						// ������ �̹��� DC
		2,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		3,
		SRCCOPY);					// ��� ȿ�� ����(�״�� ���)

}