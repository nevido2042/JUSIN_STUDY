#include "pch.h"
#include "Item.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"

CItem::CItem()
{
}

CItem::~CItem()
{
	Release();
}

void CItem::Initialize()
{
	m_eRenderID = RENDER_ITEM;
}

int CItem::Update()
{
	return CObj::Update();
}

void CItem::Late_Update()
{
	Follow_Pawn();
}

void CItem::Render(HDC hDC)
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
	float fZoom = CCamera::Get_Instance()->Get_Zoom(); // 줌 레벨 가져오기

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	// 월드 좌표를 스크린 좌표로 변환 (줌 반영)
	POINT tScreenPoint = CCamera::Get_Instance()->WorldToScreen(m_tInfo.fX, m_tInfo.fY);

	// 이미지 크기와 좌표에 줌 레벨 반영
	GdiTransparentBlt(hDC,                           // 복사 받을 DC
		(int)(tScreenPoint.x - (m_tInfo.fCX / 2) * fZoom), // 중심점 기준 좌표 조정 (가로)
		(int)(tScreenPoint.y - (m_tInfo.fCY / 2) * fZoom), // 중심점 기준 좌표 조정 (세로)
		(int)(m_tInfo.fCX * fZoom),                  // 복사 받을 이미지의 가로 (줌 반영)
		(int)(m_tInfo.fCY * fZoom),                  // 복사 받을 이미지의 세로 (줌 반영)
		hMemDC,                                      // 복사할 이미지 DC
		0,                                           // 비트맵 출력 시작 좌표 X (고정)
		0,                                           // 비트맵 출력 시작 좌표 Y (고정)
		(int)m_tInfo.fCX,                            // 복사할 이미지의 원본 가로 크기
		(int)m_tInfo.fCY,                            // 복사할 이미지의 원본 세로 크기
		RGB_WHITE);                                  // 제거할 색상

}

void CItem::Release()
{
}

void CItem::Follow_Pawn()
{
	if (!m_pTarget)
	{
		return;
	}

	Set_Pos(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
}
