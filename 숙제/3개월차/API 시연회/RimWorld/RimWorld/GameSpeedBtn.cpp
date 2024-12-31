#include "pch.h"
#include "GameSpeedBtn.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

CGameSpeedBtn::CGameSpeedBtn()
{
}

CGameSpeedBtn::~CGameSpeedBtn()
{
	Release();
}

void CGameSpeedBtn::Initialize()
{
	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 48.f;

	m_eRenderID = RENDER_INUI;
}

void CGameSpeedBtn::Late_Update()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&m_tRect, ptMouse))
	{
		if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
		{
			if (!lstrcmp(L"TimeSpeedButton_Pause", m_pImgKey))
			{
				CTimeMgr::Get_Instance()->Set_GameSpeed(0.f);
			}
			else if (!lstrcmp(L"TimeSpeedButton_Normal", m_pImgKey))
			{
				CTimeMgr::Get_Instance()->Set_GameSpeed(1.f);

			}
			else if (!lstrcmp(L"TimeSpeedButton_Fast", m_pImgKey))
			{
				CTimeMgr::Get_Instance()->Set_GameSpeed(2.f);

			}
			else if (!lstrcmp(L"TimeSpeedButton_Superfast", m_pImgKey))
			{
				CTimeMgr::Get_Instance()->Set_GameSpeed(3.f);

			}
				

			return;
		}

		//m_iDrawID = 1;
	}

	else
	{

	}
		//m_iDrawID = 0;
}

void CGameSpeedBtn::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(
		hDC,                          // 대상 DC
		m_tRect.left,                 // 출력할 X 좌표
		m_tRect.top,                  // 출력할 Y 좌표
		(int)m_tInfo.fCX,             // 출력 너비
		(int)m_tInfo.fCY,             // 출력 높이
		hMemDC,                       // 원본 DC
		m_iDrawID * (int)m_tInfo.fCX, // 원본 이미지의 X 좌표
		0,                            // 원본 이미지의 Y 좌표
		(int)m_tInfo.fCX,             // 원본 이미지의 너비
		(int)m_tInfo.fCY,             // 원본 이미지의 높이
		RGB_WHITE		              // 투명하게 처리할 색상 (예: 보라색)
	);
	
	//해당 스피드에 빨간 사각형 생성
	HPEN hPen(nullptr);
	HPEN hOldPen(nullptr);
	if (GAMESPEED == 0.f && !lstrcmp(L"TimeSpeedButton_Pause", m_pImgKey))
	{	
		// 빨간색 펜 생성
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // 굵기 1, 빨간색
		hOldPen = (HPEN)SelectObject(hDC, hPen);       // 기존 펜 저장 및 빨간색 펜 설정

		// 사각형 그리기
		MoveToEx(hDC, m_tRect.left, m_tRect.top, nullptr);         // 왼쪽 위로 이동
		LineTo(hDC, m_tRect.right , m_tRect.top);                  // 상단 선
		LineTo(hDC, m_tRect.right , m_tRect.bottom );               // 오른쪽 선
		LineTo(hDC, m_tRect.left , m_tRect.bottom );                // 하단 선
		LineTo(hDC, m_tRect.left , m_tRect.top );                   // 왼쪽 선

		// 펜 정리
		SelectObject(hDC, hOldPen); // 기존 펜 복원
		DeleteObject(hPen);         // 빨간색 펜 삭제
	}
	else if (GAMESPEED == 1.f && !lstrcmp(L"TimeSpeedButton_Normal", m_pImgKey))
	{
		// 빨간색 펜 생성
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // 굵기 1, 빨간색
		hOldPen = (HPEN)SelectObject(hDC, hPen);       // 기존 펜 저장 및 빨간색 펜 설정

		// 사각형 그리기
		MoveToEx(hDC, m_tRect.left, m_tRect.top, nullptr);         // 왼쪽 위로 이동
		LineTo(hDC, m_tRect.right, m_tRect.top);                  // 상단 선
		LineTo(hDC, m_tRect.right, m_tRect.bottom);               // 오른쪽 선
		LineTo(hDC, m_tRect.left, m_tRect.bottom);                // 하단 선
		LineTo(hDC, m_tRect.left, m_tRect.top);                   // 왼쪽 선

		// 펜 정리
		SelectObject(hDC, hOldPen); // 기존 펜 복원
		DeleteObject(hPen);         // 빨간색 펜 삭제
	}
	else if (GAMESPEED == 2.f && !lstrcmp(L"TimeSpeedButton_Fast", m_pImgKey))
	{
		// 빨간색 펜 생성
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // 굵기 1, 빨간색
		hOldPen = (HPEN)SelectObject(hDC, hPen);       // 기존 펜 저장 및 빨간색 펜 설정

		// 사각형 그리기
		MoveToEx(hDC, m_tRect.left, m_tRect.top, nullptr);         // 왼쪽 위로 이동
		LineTo(hDC, m_tRect.right, m_tRect.top);                  // 상단 선
		LineTo(hDC, m_tRect.right, m_tRect.bottom);               // 오른쪽 선
		LineTo(hDC, m_tRect.left, m_tRect.bottom);                // 하단 선
		LineTo(hDC, m_tRect.left, m_tRect.top);                   // 왼쪽 선

		// 펜 정리
		SelectObject(hDC, hOldPen); // 기존 펜 복원
		DeleteObject(hPen);         // 빨간색 펜 삭제
	}
	else if (GAMESPEED == 3.f && !lstrcmp(L"TimeSpeedButton_Superfast", m_pImgKey))
	{
		// 빨간색 펜 생성
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // 굵기 1, 빨간색
		hOldPen = (HPEN)SelectObject(hDC, hPen);       // 기존 펜 저장 및 빨간색 펜 설정

		// 사각형 그리기
		MoveToEx(hDC, m_tRect.left, m_tRect.top, nullptr);         // 왼쪽 위로 이동
		LineTo(hDC, m_tRect.right, m_tRect.top);                  // 상단 선
		LineTo(hDC, m_tRect.right, m_tRect.bottom);               // 오른쪽 선
		LineTo(hDC, m_tRect.left, m_tRect.bottom);                // 하단 선
		LineTo(hDC, m_tRect.left, m_tRect.top);                   // 왼쪽 선

		// 펜 정리
		SelectObject(hDC, hOldPen); // 기존 펜 복원
		DeleteObject(hPen);         // 빨간색 펜 삭제
	}

}

void CGameSpeedBtn::Release()
{
}

int CGameSpeedBtn::Update()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}
