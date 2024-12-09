#include "pch.h"
#include "Block.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CBlock::CBlock()
    :m_eBlockType((BLOCKTYPE)0)//이게 맞나..?
{
}

CBlock::CBlock(float _fX, float _fY, float _fSize)
    :m_eBlockType((BLOCKTYPE)0)
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

	 BitBlt(hDC,						// 복사 받을 DC
	 	m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
	 	m_tRect.top,
	 	(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
	 	(int)m_tInfo.fCY,
	 	hMemDC,						// 복사할 이미지 DC
	 	0,							// 비트맵 출력 시작 좌표(Left, top)
	 	0,
	 	SRCCOPY);					// 출력 효과 설정(그대로 출력)
}

void CBlock::Release()
{
}
