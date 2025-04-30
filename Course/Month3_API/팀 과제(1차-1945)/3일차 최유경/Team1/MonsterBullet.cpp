#include "pch.h"
#include "MonsterBullet.h"

void MonsterBullet::Render(HDC _hDC)
{
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(220, 20, 60));
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);
	Ellipse(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	SelectObject(_hDC, oldBrush);
	DeleteObject(myBrush);
}
