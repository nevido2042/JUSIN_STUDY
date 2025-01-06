#include "pch.h"
#include "Campfire.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CCampfire::CCampfire()
{
}

CCampfire::~CCampfire()
{
	Release();
}

void CCampfire::Initialize()
{
	Set_Size(64.f, 64.f);

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	//m_tFrame.ullSpeed = 200;
	m_tFrame.ullTime = 10;

	m_eRenderID = RENDER_GAMEOBJECT;
}

void CCampfire::Late_Update()
{
	__super::Move_Frame();
}

void CCampfire::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hFireDC = CBmpMgr::Get_Instance()->Find_Image(L"FireAnimated");
	HDC hCampfireDC = CBmpMgr::Get_Instance()->Find_Image(L"Campfire");

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hCampfireDC,						// 복사할 이미지 DC	
		0,							// 비트맵 출력 시작 좌표(Left, top)
		0,
		(int)m_tInfo.fCX,			// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB_WHITE);		// 제거할 색상

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX + 5,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY - 5,
		(int)(m_tInfo.fCX * 0.9f),			// 복사 받을 이미지의 가로, 세로
		(int)(m_tInfo.fCY * 0.9f),
		hFireDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_tFrame.iFrameStart,							// 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * m_tFrame.iMotion,
		(int)m_tInfo.fCX,			// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB_PURPLE);		// 제거할 색상
}

void CCampfire::Release()
{
}
