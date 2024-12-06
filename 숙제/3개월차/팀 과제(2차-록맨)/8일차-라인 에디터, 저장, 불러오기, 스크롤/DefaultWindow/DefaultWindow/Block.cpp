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

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/maja2.bmp", L"Block");
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

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Block");

	 BitBlt(hDC,						// ���� ���� DC
	 	m_tRect.left + iScrollX,	// ���� ���� ��ġ ��ǥ X, Y	
	 	m_tRect.top,
	 	(int)m_tInfo.fCX,			// ���� ���� �̹����� ����, ����
	 	(int)m_tInfo.fCY,
	 	hMemDC,						// ������ �̹��� DC
	 	0,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
	 	0,
	 	SRCCOPY);					// ��� ȿ�� ����(�״�� ���)
}

void CBlock::Release()
{
}
