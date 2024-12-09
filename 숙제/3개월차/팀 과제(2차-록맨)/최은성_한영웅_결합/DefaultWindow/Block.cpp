#include "pch.h"
#include "Block.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CLineMgr.h"

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
	
	//__super::Update_Rect();
	//CLineMgr::Get_Instance()->Add_Line(m_tRect);

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

	 //BitBlt(hDC,						// 복사 받을 DC
	 //	m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
	 //	m_tRect.top + iScrollY,// 복사 받을 위치 좌표 X, Y	
	 //	(int)m_tInfo.fCX,		// 복사 받을 이미지의 가로, 세로
	 //	(int)m_tInfo.fCY,
	 //	hMemDC,						// 복사할 이미지 DC
	 //	0,							// 비트맵 출력 시작 좌표(Left, top)
	 //	0,
	 //	SRCCOPY);					// 출력 효과 설정(그대로 출력)
	GdiTransparentBlt(
		hDC,                        // 복사 받을 DC (화면에 출력할 DC)
		m_tRect.left + iScrollX,    // 복사받을 위치의 X좌표 (현재 스크롤 값을 고려하여 계산)
		m_tRect.top + iScrollY,     // 복사받을 위치의 Y좌표 (현재 스크롤 값을 고려하여 계산)
		33,                         // 복사받을 이미지의 가로 길이 (출력할 이미지 크기 지정)
		33,                         // 복사받을 이미지의 세로 길이
		hMemDC,                     // 복사할 이미지가 저장된 메모리 DC
		206,                        // 원본 비트맵에서 복사를 시작할 X좌표
		3,                        // 원본 비트맵에서 복사를 시작할 Y좌표
		33,                         // 원본 비트맵에서 복사할 영역의 가로 길이
		33,                         // 원본 비트맵에서 복사할 영역의 세로 길이
		RGB(128, 0, 128));       // 투명 처리할 색상 (이미지의 이 색상이 투명 처리됨)

}

void CBlock::Release()
{
}
