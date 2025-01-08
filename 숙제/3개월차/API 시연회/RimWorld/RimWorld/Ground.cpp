#include "pch.h"
#include "Ground.h"
#include "BmpMgr.h"
#include "Camera.h"

CGround::CGround()
	:m_pImage(nullptr)
{
}

CGround::~CGround()
{
	Release();
}

void CGround::Initialize()
{
	// GDI+로 PNG 이미지 로드
	//m_pImage = Gdiplus::Image::FromFile(L"../Image/Terrain/Gravel.png");

	Set_Size(TILECX * TILEX, TILECY * TILEY);

	m_eRenderID = RENDER_BACKGROUND;
}

void CGround::Late_Update()
{
}

void CGround::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Gravel");

	// 줌을 적용한 타일 크기 계산
	float fZoom = CCamera::Get_Instance()->Get_Zoom();
	POINT tPoint = CCamera::Get_Instance()->WorldToScreen(m_tRect.left, m_tRect.top);
	// 타일 렌더링 (StretchBlt 사용)
	StretchBlt(
		hDC,
		tPoint.x,
		tPoint.y,
		(int)(m_tInfo.fCX * fZoom),
		(int)(m_tInfo.fCY * fZoom),
		hMemDC,
		0,
		0,
		m_tInfo.fCX,
		m_tInfo.fCY,
		SRCCOPY // 단순 복사
	);

	//// GDI+ Graphics 객체 생성
	//Gdiplus::Graphics graphics(hDC);

	//// 줌과 화면 좌표 계산
	//float fZoom = CCamera::Get_Instance()->Get_Zoom();
	//POINT tPoint = CCamera::Get_Instance()->WorldToScreen(m_tRect.left, m_tRect.top);

	//// 출력 영역 계산 (줌 반영)
	//Gdiplus::RectF destRect(
	//	(float)tPoint.x,
	//	(float)tPoint.y,
	//	m_tInfo.fCX * fZoom,
	//	m_tInfo.fCY * fZoom
	//);

	//// PNG 이미지 렌더링
	//graphics.DrawImage(m_pImage, destRect);
}

void CGround::Release()
{
}
