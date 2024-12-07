#include "pch.h"
#include "Boss_IceMan.h"
#include "CScrollMgr.h"

CBoss_IceMan::CBoss_IceMan()
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
}

int CBoss_IceMan::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Set_PosY(5.f);


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
