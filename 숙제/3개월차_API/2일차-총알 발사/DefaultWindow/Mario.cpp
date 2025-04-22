#include "pch.h"
#include "Mario.h"

CMario::CMario()
	:m_bJump(false), m_ullJumpTime(0), m_bGround(false), m_fFallSpeed(0), m_fGravity(0)
{
}

void CMario::Initialize()
{
	m_tInfo = { 0, 0, 20.f, 40.f };
	Set_Pos(MINI_WINCX_RIGHT * 0.5f, MINI_WINCY_BOTTOM - Get_Info().fCY * 0.5f);
	m_fSpeed = 5.f;
	m_bJump = false;
	m_bGround = true;
	m_fGravity = 0.98f;
}

void CMario::Update()
{
	Key_Input();

	if (m_bJump)
	{
		m_tInfo.fY -= m_fFallSpeed;
		m_fFallSpeed -= m_fGravity;

		if (m_fFallSpeed < 0/*m_ullJumpTime + 300 < GetTickCount64()*/)
		{
			m_bJump = false;
			m_fFallSpeed = 0.f;
		}
	}
	else
	{
		if (MINI_WINCY_BOTTOM > Get_Info().fY + Get_Info().fCY * 0.5f)//땅에 안닿았으면
		{
			m_bGround = false;
			m_tInfo.fY += m_fFallSpeed;
			m_fFallSpeed += m_fGravity;
		}
		else
		{
			Set_Pos(Get_Info().fX, MINI_WINCY_BOTTOM - Get_Info().fCY * 0.5f);
			m_bGround = true;
			m_fFallSpeed = 0.f;
		}
	}

	// CObj::Update_Rect();
	__super::Update_Rect();
}

void CMario::Render(HDC hDC)
{
	Rectangle(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tRect.right,
		m_tRect.bottom);
}

void CMario::Release()
{
}

void CMario::Key_Input()
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

	if (GetAsyncKeyState(VK_SPACE))
	{
		if (!m_bJump && m_bGround)//점프 중이 아니고 땅에 닿았을 때만
		{
			m_bJump = true;
			m_bGround = false;
			m_ullJumpTime = GetTickCount64();
			m_fFallSpeed = 9.8f * 2.f;
		}

	}
}
