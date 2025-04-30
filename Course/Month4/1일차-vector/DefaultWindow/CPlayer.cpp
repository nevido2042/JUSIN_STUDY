#include "pch.h"
#include "CPlayer.h"

CPlayer::CPlayer()
	:m_LastMoveTime(0)
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo.vPos = { 400.f, 300.f, 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_fSpeed = 10.f;
}

void CPlayer::Update()
{
	//m_tInfo.vPos += m_tInfo.vDir;

	if (m_LastMoveTime + 1000 < GetTickCount64())
	{
		m_LastMoveTime = GetTickCount64();
		m_tInfo.vPos.x = rand() % WINCX;
		m_tInfo.vPos.y = rand() % WINCY;
		m_tInfo.vPos.z = 0.f;
	}
}

void CPlayer::Render(HDC hDC)
{
	Rectangle(hDC, 
		int(m_tInfo.vPos.x - 50.f), 
		int(m_tInfo.vPos.y - 50.f), 
		int(m_tInfo.vPos.x + 50.f),
		int(m_tInfo.vPos.y + 50.f));
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()
{
}
