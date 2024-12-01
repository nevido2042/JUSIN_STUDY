#include "pch.h"
#include "FireRate_Item.h"
#include "Player.h"

void FireRate_Item::Upgrade_Target()
{
	static_cast<Player*>(m_pTarget)->Add_FireRate(-5);
}

void FireRate_Item::Render(HDC _hDC)
{
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(255, 255, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);
	Rectangle(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	SelectObject(_hDC, oldBrush);
	DeleteObject(myBrush);
}
