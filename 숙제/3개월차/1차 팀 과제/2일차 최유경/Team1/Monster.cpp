#include "pch.h"
#include "Monster.h"
#include "FireRate_Item.h"
#include "Bullet_Item.h"

Monster::Monster():m_dir(NODIR), m_ItemList(nullptr)
{
}

Monster::~Monster()
{
	Release();
}

void Monster::Initialize()
{
	m_tInfo.fCX = 20.f;
	m_tInfo.fCY = 20.f;
	m_fSpeed = 0.5f;
	m_iDamage = 1;
	m_iHp = 5;

	m_dir = rand() % 4;
	int iXorY = 0;

	switch (m_dir)
	{
	case LEFT:
		iXorY = rand() % (int)GAME_WINCY + (int)GAME_WIN_TOP;
		m_tInfo.fX = (int)GAME_WIN_LEFT + m_tInfo.fCX;
		m_tInfo.fY = (float)iXorY;
		break;
	case TOP:
		iXorY = rand() % (int)GAME_WINCX + (int)GAME_WIN_LEFT;
		m_tInfo.fY = (int)GAME_WIN_TOP + m_tInfo.fCY;
		m_tInfo.fX = (float)iXorY;
		break;
	case RIGHT:
		iXorY = rand() % (int)GAME_WINCY + (int)GAME_WIN_TOP;
		m_tInfo.fX = (int)GAME_WIN_RIGHT - m_tInfo.fCX;
		m_tInfo.fY = (float)iXorY;
		break;
	case BOTTOM:
		iXorY = rand() % (int)GAME_WINCX + (int)GAME_WIN_LEFT;
		m_tInfo.fY = (int)GAME_WIN_BOTTOM - m_tInfo.fCY;
		m_tInfo.fX = (float)iXorY;
		break;
	case NODIR:
		break;
	default:
		break;
	}
}

int Monster::Update()
{

	if (m_bDead) {
		return OBJ_DEAD;
	}

	m_fSpeed += 0.001f;

	Obj::Update_Rect();

	return OBJ_NOEVENT;
}

void Monster::Late_Update()
{
	if (m_iHp <= 0) {
		m_bDead = true;

		Drop_Item();
	}

	//Target을 추적한다.
	float fDiagonal(0), fWidth(0), fHeight(0), fRadian(0);

	fWidth = m_pTarget->Get_Info().fX - m_tInfo.fX;
	fHeight = m_pTarget->Get_Info().fY - m_tInfo.fY;
	fDiagonal = sqrtf((float)pow(fWidth, 2) + (float)pow(fHeight, 2));
	fRadian = acosf(fWidth / fDiagonal);

	m_fAngle = fRadian * (180.f / PI);

	if (m_pTarget->Get_Info().fY > m_tInfo.fY)
	{
		//m_fAngle *= -1.f;
		m_fAngle = 360.f - m_fAngle;
	}

	m_tInfo.fX += m_fSpeed * ((float)cos(m_fAngle * (PI / 180.f)));
	m_tInfo.fY -= m_fSpeed * ((float)sin(m_fAngle * (PI / 180.f)));
}

void Monster::Render(HDC _hdc)
{
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(0, 103, 163));
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, myBrush);
	Rectangle(_hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	SelectObject(_hdc, oldBrush);
	DeleteObject(myBrush);
}

void Monster::Release()
{
}

void Monster::Drop_Item()
{
	int iRandDrop = rand() % 100;
	if (iRandDrop < 80)
	{
		int iRandItem = rand() % (int)ITEM_END;
		Item* pItem(nullptr);
		switch (iRandItem)
		{
		case FIRERATE:
			pItem = new FireRate_Item;
			break;
		case BULLET:
			pItem = new Bullet_Item;
			break;
		case ITEM_END:
			break;
		default:
			break;
		}

		pItem->Set_Pos(Get_Info().fX, Get_Info().fY);
		pItem->Set_Target(m_pTarget);
		m_ItemList->push_back(pItem);
		m_ItemList->back()->Initialize();
	}
}
