#include "pch.h"
#include "Heal_Item.h"

void Heal_Item::Upgrade_Target()
{
	m_pTarget->Add_HP(5);
}

void Heal_Item::Render(HDC _hDC)
{
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(0, 255, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);
	Rectangle(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	SelectObject(_hDC, oldBrush);
	DeleteObject(myBrush);
}
