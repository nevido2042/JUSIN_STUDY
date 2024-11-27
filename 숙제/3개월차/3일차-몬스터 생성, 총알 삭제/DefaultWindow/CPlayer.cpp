#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CAbstractFactory.h"

CPlayer::CPlayer() : m_pBullet(nullptr)
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo  = { WINCX / 2.f, WINCY / 2.f, 100.f, 100.f };
	m_fSpeed = 10.f;
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
}


void CPlayer::Render(HDC hDC)
{
	Rectangle(hDC, 
		m_tRect.left, 
		m_tRect.top, 
		m_tRect.right, 
		m_tRect.bottom);
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

	if (GetAsyncKeyState(VK_SPACE))
	{
		m_pBullet->push_back(Create_Bullet(DIR_UP));
		//m_pBullet->push_back(Create_Bullet(DIR_LU));
		//m_pBullet->push_back(Create_Bullet(DIR_RU));
	}

	/*if (GetAsyncKeyState('W')) m_pBullet->push_back(Create_Bullet(DIR_UP));
	if (GetAsyncKeyState('S')) m_pBullet->push_back(Create_Bullet(DIR_DOWN));
	if (GetAsyncKeyState('A')) m_pBullet->push_back(Create_Bullet(DIR_LEFT));
	if (GetAsyncKeyState('D'))
	{
		m_pBullet->push_back(CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY, DIR_RIGHT));
	}*/
}

CObj* CPlayer::Create_Bullet(DIRECTION eDir)
{
	CObj* pBullet = CAbstractFactory<CBullet>::Create();
	pBullet->Set_Pos(m_tInfo.fX, m_tInfo.fY);
	pBullet->Set_Direction(eDir);

	return pBullet;
}

