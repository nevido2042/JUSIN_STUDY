#include "pch.h"
#include "Boss_IceMan.h"
#include "CScrollMgr.h"

CBoss_IceMan::CBoss_IceMan()
	: m_fAccel(0.f), m_fGravity(0.f)
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

	m_fAccel = 0.1f;
	m_fGravity = 9.8f;
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
	m_fGravity += m_fAccel;
	Set_PosY(m_fGravity);
}

void CBoss_IceMan::Jump()
{
}

void CBoss_IceMan::Fire()
{
}
