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

    float fZoom = CCamera::Get_Instance()->Get_Zoom();

    HDC hFireDC = CBmpMgr::Get_Instance()->Find_Image(L"FireAnimated");
    HDC hCampfireDC = CBmpMgr::Get_Instance()->Find_Image(L"Campfire");

    // 월드 좌표를 스크린 좌표로 변환 (줌 반영)
    POINT tScreenPoint = CCamera::Get_Instance()->WorldToScreen(m_tInfo.fX, m_tInfo.fY);

    // 첫 번째 이미지: Campfire
    GdiTransparentBlt(hDC,
        (int)(tScreenPoint.x - (m_tInfo.fCX / 2) * fZoom),  // 중심점 기준 좌표 조정 (가로)
        (int)(tScreenPoint.y - (m_tInfo.fCY / 2) * fZoom),  // 중심점 기준 좌표 조정 (세로)
        (int)(m_tInfo.fCX * fZoom),                        // 복사받을 이미지의 가로 (줌 반영)
        (int)(m_tInfo.fCY * fZoom),                        // 복사받을 이미지의 세로 (줌 반영)
        hCampfireDC,
        0,
        0,
        (int)m_tInfo.fCX,                                  // 원본 가로 크기
        (int)m_tInfo.fCY,                                  // 원본 세로 크기
        RGB_WHITE);

    // 두 번째 이미지: FireAnimated
    GdiTransparentBlt(hDC,
        (int)(tScreenPoint.x - (m_tInfo.fCX * 0.45f) * fZoom),  // 좌표 조정 (가로) + 줌 반영
        (int)(tScreenPoint.y - (m_tInfo.fCY * 0.55f) * fZoom),  // 좌표 조정 (세로) + 줌 반영
        (int)((m_tInfo.fCX * 0.9f) * fZoom),                   // 복사받을 이미지의 가로 (줌 반영)
        (int)((m_tInfo.fCY * 0.9f) * fZoom),                   // 복사받을 이미지의 세로 (줌 반영)
        hFireDC,
        (int)(m_tInfo.fCX * m_tFrame.iFrameStart),             // 비트맵 출력 시작 좌표 (X)
        (int)(m_tInfo.fCY * m_tFrame.iMotion),                 // 비트맵 출력 시작 좌표 (Y)
        (int)m_tInfo.fCX,                                      // 원본 가로 크기
        (int)m_tInfo.fCY,                                      // 원본 세로 크기
        RGB_PURPLE);

}

void CCampfire::Release()
{
}
