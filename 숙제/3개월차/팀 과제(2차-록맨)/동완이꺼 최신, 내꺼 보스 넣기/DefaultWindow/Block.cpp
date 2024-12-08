#include "pch.h"
#include "Block.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CBlock::CBlock()
{
}

CBlock::CBlock(float _fX, float _fY, float _fSize)
{
    Set_Pos(_fX, _fX);
    Set_Size(_fSize, _fSize);
}

CBlock::~CBlock()
{
}

void CBlock::Initialize()
{
    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 50.f;

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/tile_fire.bmp", L"Block");
}

int CBlock::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CBlock::Late_Update()
{
}

void CBlock::Render(HDC hDC)
{
    //int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

    /*Rectangle(hDC,
        m_tRect.left + iScrollX,
        m_tRect.top,
        m_tRect.right + iScrollX,
        m_tRect.bottom);*/

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Block");

	 //BitBlt(hDC,						// ���� ���� DC
	 //	m_tRect.left + iScrollX,	// ���� ���� ��ġ ��ǥ X, Y	
	 //	m_tRect.top + iScrollY,// ���� ���� ��ġ ��ǥ X, Y	
	 //	(int)m_tInfo.fCX,		// ���� ���� �̹����� ����, ����
	 //	(int)m_tInfo.fCY,
	 //	hMemDC,						// ������ �̹��� DC
	 //	0,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
	 //	0,
	 //	SRCCOPY);					// ��� ȿ�� ����(�״�� ���)
	GdiTransparentBlt(
		hDC,                        // ���� ���� DC (ȭ�鿡 ����� DC)
		m_tRect.left + iScrollX,    // ������� ��ġ�� X��ǥ (���� ��ũ�� ���� ����Ͽ� ���)
		m_tRect.top + iScrollY,     // ������� ��ġ�� Y��ǥ (���� ��ũ�� ���� ����Ͽ� ���)
		33,                         // ������� �̹����� ���� ���� (����� �̹��� ũ�� ����)
		33,                         // ������� �̹����� ���� ����
		hMemDC,                     // ������ �̹����� ����� �޸� DC
		206,                        // ���� ��Ʈ�ʿ��� ���縦 ������ X��ǥ
		3,                        // ���� ��Ʈ�ʿ��� ���縦 ������ Y��ǥ
		33,                         // ���� ��Ʈ�ʿ��� ������ ������ ���� ����
		33,                         // ���� ��Ʈ�ʿ��� ������ ������ ���� ����
		RGB(128, 0, 128));       // ���� ó���� ���� (�̹����� �� ������ ���� ó����)

}

void CBlock::Release()
{
}
