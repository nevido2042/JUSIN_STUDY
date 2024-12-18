#include "pch.h"
#include "TimeMgr.h"

CTimeMgr* CTimeMgr::m_pInstance = nullptr;

CTimeMgr::CTimeMgr()
	:m_iCurFrame(0), m_fGameSpeed(0.f), m_fElapsedTime(0.f)
{

}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::Initialize()
{	
	m_iCurFrame = 0;
	m_fGameSpeed = 1.f;
}

void CTimeMgr::Update()
{
	++m_iCurFrame;
	m_fElapsedTime += Get_GameSpeed() * 0.01f;
}

void CTimeMgr::Render(HDC hDC)
{
	// 변수 값을 유니코드 문자열로 변환
	wchar_t buffer[50];
	//wsprintf(buffer, L"m_iCurFrame: %d", m_iCurFrame);
	//// 문자열 출력 (유니코드)
	//TextOutW(hDC, 0, 0, buffer, lstrlenW(buffer));

	wsprintf(buffer, L"m_fElapsedTime: %d", (int)m_fElapsedTime);
	// 문자열 출력 (유니코드)
	TextOutW(hDC, 0, 0, buffer, lstrlenW(buffer));
}
