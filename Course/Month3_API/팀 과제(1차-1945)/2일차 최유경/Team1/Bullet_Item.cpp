#include "pch.h"
#include "Bullet_Item.h"
#include "Player.h"

void Bullet_Item::Upgrade_Target()
{
	Player* pPlayer = static_cast<Player*>(m_pTarget);
	switch (pPlayer->Get_BulletType())
	{
	case BULLET_ONE:
		pPlayer->Set_BulletType(BULLET_TWO);
		break;
	case BULLET_TWO:
		pPlayer->Set_BulletType(BULLET_THREE);
		break;
	case BULLET_THREE:
		pPlayer->Set_BulletType(BULLET_SCREW);
		break;
	case BULLET_SCREW:
		break;
	}
}

void Bullet_Item::Render(HDC _hDC)
{
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(165, 42, 42));
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);
	Rectangle(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	SelectObject(_hDC, oldBrush);
	DeleteObject(myBrush);
}
