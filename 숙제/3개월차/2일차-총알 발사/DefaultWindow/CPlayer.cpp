#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"

CPlayer::CPlayer() : m_pBulletList(nullptr)
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

void CPlayer::Update()
{
	Key_Input();

	// CObj::Update_Rect();
	__super::Update_Rect();
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

	if (GetAsyncKeyState('W'))
	{
		CObj* pObj = Create_Bullet();
		if (CBullet* pBullet = dynamic_cast<CBullet*>(pObj))
		{
			pBullet->Set_Direction(CBullet::Direction::UP);
		}
		m_pBulletList->push_back(pObj);
	}

	if (GetAsyncKeyState('A'))
	{
		CObj* pObj = Create_Bullet();
		if (CBullet* pBullet = dynamic_cast<CBullet*>(pObj))
		{
			pBullet->Set_Direction(CBullet::Direction::LEFT);
		}
		m_pBulletList->push_back(pObj);
	}

	if (GetAsyncKeyState('S'))
	{
		CObj* pObj = Create_Bullet();
		if (CBullet* pBullet = dynamic_cast<CBullet*>(pObj))
		{
			pBullet->Set_Direction(CBullet::Direction::DOWN);
		}
		m_pBulletList->push_back(pObj);
	}

	if (GetAsyncKeyState('D'))
	{
		CObj* pObj = Create_Bullet();
		if (CBullet* pBullet = dynamic_cast<CBullet*>(pObj))
		{
			pBullet->Set_Direction(CBullet::Direction::RIGHT);
		}
		m_pBulletList->push_back(pObj);
	}
}

CObj* CPlayer::Create_Bullet()
{
	CObj* pBullet = new CBullet;

	pBullet->Initialize();
	pBullet->Set_Pos(m_tInfo.fX, m_tInfo.fY);

	return pBullet;
}
