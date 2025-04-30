#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"

CPlayer::CPlayer() : m_pBulletList(nullptr), m_ullLast_Fire(0)
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo  = { WINCX / 2.f, WINCY / 2.f, 75.f, 75.f };
	m_fSpeed = 5.f;
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

	//총알 발사 대기 시간
	if (m_ullLast_Fire + 100 > GetTickCount64())
	{
		return;
	}

	if (GetAsyncKeyState('W'))
	{
		m_ullLast_Fire = GetTickCount64();
		CObj* pObj = Create_Bullet();
		if (CBullet* pBullet = dynamic_cast<CBullet*>(pObj))
		{
			pBullet->Set_Direction(CBullet::Direction::UP);
		}
		m_pBulletList->push_back(pObj);
	}

	if (GetAsyncKeyState('A'))
	{
		m_ullLast_Fire = GetTickCount64();
		CObj* pObj = Create_Bullet();
		if (CBullet* pBullet = dynamic_cast<CBullet*>(pObj))
		{
			pBullet->Set_Direction(CBullet::Direction::LEFT);
		}
		m_pBulletList->push_back(pObj);
	}

	if (GetAsyncKeyState('S'))
	{
		m_ullLast_Fire = GetTickCount64();
		CObj* pObj = Create_Bullet();
		if (CBullet* pBullet = dynamic_cast<CBullet*>(pObj))
		{
			pBullet->Set_Direction(CBullet::Direction::DOWN);
		}
		m_pBulletList->push_back(pObj);
	}

	if (GetAsyncKeyState('D'))
	{
		m_ullLast_Fire = GetTickCount64();
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
