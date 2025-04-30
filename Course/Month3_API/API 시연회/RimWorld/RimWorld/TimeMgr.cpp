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
	// ���� ���� �����ڵ� ���ڿ��� ��ȯ
	wchar_t buffer[50];
	//wsprintf(buffer, L"m_iCurFrame: %d", m_iCurFrame);
	//// ���ڿ� ��� (�����ڵ�)
	//TextOutW(hDC, 0, 0, buffer, lstrlenW(buffer));

	wsprintf(buffer, L"m_fElapsedTime: %d", (int)m_fElapsedTime);
	// ���ڿ� ��� (�����ڵ�)
	TextOutW(hDC, 0, 0, buffer, lstrlenW(buffer));
}
