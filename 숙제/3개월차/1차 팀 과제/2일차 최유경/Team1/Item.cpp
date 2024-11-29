#include "pch.h"
#include "Item.h"

void Item::Initialize()
{
	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 10.f;
	m_fSpeed = 0.f;

	m_iHp = 1;
}

int Item::Update()
{
	if (m_bDead) {
		return OBJ_DEAD;
	}

	Obj::Update_Rect();

	return OBJ_NOEVENT;
}

void Item::Late_Update()
{
	if (m_iHp <= 0) {
		m_bDead = true;

		Upgrade_Target();
	}
}

void Item::Render(HDC _hdc)
{
	/*HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(255, 255, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, myBrush);
	Rectangle(_hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	SelectObject(_hdc, oldBrush);
	DeleteObject(myBrush);*/
}

void Item::Release()
{
}

void Item::Upgrade_Target()
{
	//m_pTarget->업그레이드

	//static_cast<Player*>(m_pTarget)->Add_FireRate(-5);
}
