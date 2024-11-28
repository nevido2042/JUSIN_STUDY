#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CAbstractFactory.h"

CPlayer::CPlayer() : m_pBullet(nullptr), m_fGunLength(0.f)
{
	ZeroMemory(&m_tMuzzlePos, sizeof(POINT));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo  = { WINCX / 2.f, WINCY / 2.f, 25.f, 25.f };
	m_fSpeed = 10.f;

	m_fGunLength = 20.f;
}

int CPlayer::Update()
{
	Key_Input();

	// CObj::Update_Rect();
	__super::Update_Rect();

	return OBJ_NOEVENT;
}
void CPlayer::Late_Update()
{
	m_tMuzzlePos.x = long(m_tInfo.fX + (m_fGunLength * cosf(m_fAngle * (PI / 180.f))));
	m_tMuzzlePos.y = long(m_tInfo.fY - (m_fGunLength * sinf(m_fAngle * (PI / 180.f))));
}


void CPlayer::Render(HDC hDC)
{
	Rectangle(hDC, 
		m_tRect.left, 
		m_tRect.top, 
		m_tRect.right, 
		m_tRect.bottom);

	// 포신
	MoveToEx(hDC, (int)m_tInfo.fX, (int)m_tInfo.fY, nullptr);
	LineTo(hDC, m_tMuzzlePos.x, m_tMuzzlePos.y);
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()
{
	// GetKeyState
	// GetAsyncKeyState // 함수 호출 시 반환되는 값에 따른 키 값 처리 가능

	if (GetAsyncKeyState(VK_RIGHT))
	{
		m_tInfo.fX += m_fSpeed;
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		m_tInfo.fX -= m_fSpeed;
	}

	if (GetAsyncKeyState(VK_UP))
	{
		m_tInfo.fY -= m_fSpeed;
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		m_tInfo.fY += m_fSpeed;
	}

	Clamp_PlayerToScreen();

	if (GetAsyncKeyState(VK_SPACE))
	{
		m_pBullet->push_back(Create_Bullet(DIR_UP)); 
	}

	if (GetAsyncKeyState('A'))
	{
		m_fAngle += 5.f;
	}

	if (GetAsyncKeyState('D'))
	{
		m_fAngle -= 5.f;
	}

}

CObj* CPlayer::Create_Bullet(DIRECTION eDir)
{
	CObj* pBullet = CAbstractFactory<CBullet>::Create();
	pBullet->Set_Pos(m_tMuzzlePos.x, m_tMuzzlePos.y);
	//pBullet->Set_Direction(eDir);
	pBullet->Set_Angle(m_fAngle);

	return pBullet;
}

void CPlayer::Clamp_PlayerToScreen()
{
	//맵 밖을 나갈 수 없도록
	if (GAME_WIN_LEFT + m_tInfo.fCX * 0.5f > m_tInfo.fX)
	{
		m_tInfo.fX = GAME_WIN_LEFT + m_tInfo.fCX * 0.5f;
	}

	if (GAME_WIN_RIGHT - m_tInfo.fCX * 0.5f < m_tInfo.fX)
	{
		m_tInfo.fX = GAME_WIN_RIGHT - m_tInfo.fCX * 0.5f;
	}

	if (GAME_WIN_TOP + m_tInfo.fCY * 0.5f > m_tInfo.fY)
	{
		m_tInfo.fY = GAME_WIN_TOP + m_tInfo.fCY * 0.5f;
	}

	if (GAME_WIN_BOTTOM - m_tInfo.fCY * 0.5f < m_tInfo.fY)
	{
		m_tInfo.fY = GAME_WIN_BOTTOM - m_tInfo.fCY * 0.5f;
	}
}

