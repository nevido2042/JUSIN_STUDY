#include "pch.h"
#include "Block_Cut.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

void CBlock_Cut::Initialize()
{
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	m_eBlockType = BLOCK_CUT;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/tile_cut.bmp", L"Tile_Cut");
}

void CBlock_Cut::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Cut");

	BitBlt(hDC,						// ���� ���� DC
		m_tRect.left + iScrollX,	// ���� ���� ��ġ ��ǥ X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// ���� ���� �̹����� ����, ����
		(int)m_tInfo.fCY,
		hMemDC,						// ������ �̹��� DC
		206,						// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		3,
		SRCCOPY);					// ��� ȿ�� ����(�״�� ���)

}