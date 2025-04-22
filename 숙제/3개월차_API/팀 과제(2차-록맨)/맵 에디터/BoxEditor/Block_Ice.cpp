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

	BitBlt(hDC,						// ���� ���� DC
		m_tRect.left + iScrollX,	// ���� ���� ��ġ ��ǥ X, Y	
		m_tRect.top + iScrollY ,
		(int)m_tInfo.fCX,			// ���� ���� �̹����� ����, ����
		(int)m_tInfo.fCY,
		hMemDC,						// ������ �̹��� DC
		35,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		163,
		SRCCOPY);					// ��� ȿ�� ����(�״�� ���)

}
