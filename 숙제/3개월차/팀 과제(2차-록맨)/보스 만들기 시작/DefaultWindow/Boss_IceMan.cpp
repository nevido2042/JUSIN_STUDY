#include "pch.h"
#include "Boss_IceMan.h"
#include "CScrollMgr.h"

CBoss_IceMan::CBoss_IceMan()
	: /*m_fAccel(0.f),*/ m_fGravity(0.f), /*m_fFallSpeed(0.f),*/ m_bJump(false),
	m_fJumpSpeed(0.f), m_fTime(0.f), m_ullJumpTimer(0.f), m_fPrevY(0.f)
{
}

CBoss_IceMan::~CBoss_IceMan()
{
	Release();
}

void CBoss_IceMan::Initialize()
{
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;
	m_fSpeed = 3.f;

	//m_fAccel = 0.1f;
	m_fGravity = 2.f;
	//m_fFallSpeed = m_fGravity;
	m_fJumpSpeed = 15.f;
	m_ullJumpTimer = GetTickCount64();
}

int CBoss_IceMan::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Fall();

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBoss_IceMan::Late_Update()
{
	if (m_ullJumpTimer + 100.f < GetTickCount64())
	{
		m_ullJumpTimer = GetTickCount64();
		m_bJump = true;
	}

	Jumping();
}

void CBoss_IceMan::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	// ������ �귯�� ����
	HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 0, 0)); // ������ �귯�� ����
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hRedBrush); // ���� �귯�� ���� �� �� �귯�� ����

	// �簢�� �׸���
	Rectangle(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		m_tRect.right + iScrollX,
		m_tRect.bottom + iScrollY);

	// ���� �귯�� ����
	SelectObject(hDC, hOldBrush);
	DeleteObject(hRedBrush); // ����� �귯�� ����
}

void CBoss_IceMan::Release()
{
}

void CBoss_IceMan::Fall()
{
	if (!m_bJump)
	{
		//m_fFallSpeed += m_fAccel;
		Set_PosY(m_fGravity);

		if (m_bGround)
		{
			m_fGravity;
		}
	}

}

void CBoss_IceMan::Jumping()
{
	if (m_bJump)
	{
		m_tInfo.fX -= cosf(45.f) * m_fTime;
		m_tInfo.fY -= (m_fJumpSpeed * sinf(90.f) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
		m_fTime += 0.1f;

		//�� �������� ���̿� ���� ���̸� ��
		//�������� ������ ���� ����
		if (m_fPrevY < m_tInfo.fY)
		{
			m_bJump = false;
			m_fTime = 0.f;
		}

		m_fPrevY = m_tInfo.fY;
		Update_Rect();
	}
}

void CBoss_IceMan::Fire()
{
}
