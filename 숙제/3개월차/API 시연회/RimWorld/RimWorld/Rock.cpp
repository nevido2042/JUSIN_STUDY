#include "pch.h"
#include "Rock.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CRock::CRock()
	:m_eCurState(END), m_ePreState(END)
{
}

CRock::~CRock()
{
    Release();
}

void CRock::Change_Image()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CRock::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.ullSpeed = 200;
			m_tFrame.ullTime = GetTickCount64();
			break;

		case CRock::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 1;
			m_tFrame.ullSpeed = 200;
			m_tFrame.ullTime = GetTickCount64();
			break;

		case CRock::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 2;
			m_tFrame.ullSpeed = 200;
			m_tFrame.ullTime = GetTickCount64();
			break;

		case CRock::HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 3;
			m_tFrame.ullSpeed = 200;
			m_tFrame.ullTime = GetTickCount64();
			break;

		case CRock::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 4;
			m_tFrame.ullSpeed = 200;
			m_tFrame.ullTime = GetTickCount64();
			break;
		}

		m_ePreState = m_eCurState;
	}

}

void CRock::Initialize()
{
	Set_ImgKey(L"Rock_Atlas");
	//m_pImgKey = L"Rock_Atlas";

	m_tInfo.fCX = 80.f; //나중에 오프셋 64(충돌범위)+16(렌더) 이런식으로 줘야할듯
    m_tInfo.fCY = 80.f;

	m_eCurState = IDLE;
	m_ePreState = IDLE;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 3;
	m_tFrame.ullSpeed = 500
		;
	m_tFrame.ullTime = GetTickCount64();

	m_eRender = RENDER_GAMEOBJECT;
}

int CRock::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CRock::Late_Update()
{
	__super::Move_Frame();
}

void CRock::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	/*Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
		m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	BitBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,		
		(int)m_tInfo.fCY,
		hMemDC,					
		(int)m_tInfo.fCX * m_tFrame.iFrameStart,
		(int)m_tInfo.fCY * m_tFrame.iMotion,
		SRCCOPY);*/

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_tFrame.iFrameStart,// 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * m_tFrame.iMotion,
		(int)m_tInfo.fCX,			// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB_PURPLE);		// 제거할 색상
}

void CRock::Release()
{
}
