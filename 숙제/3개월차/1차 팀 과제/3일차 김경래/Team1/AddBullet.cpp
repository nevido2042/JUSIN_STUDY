#include "pch.h"
#include "AddBullet.h"
#include "Player.h"

AddBullet::AddBullet()
{
}

AddBullet::~AddBullet()
{
}

void AddBullet::Initialize()
{
    
    m_tInfo.fCX = 20.f;
    m_tInfo.fCY = 20.f;
    m_iHp = 1;
}

int AddBullet::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    Obj::Update_Rect();
    return OBJ_NOEVENT;
}

void AddBullet::Late_Update()
{
}

void AddBullet::Render(HDC _hdc)
{
    HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(150, 100, 60));
    HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, myBrush);
    Ellipse(_hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
    SelectObject(_hdc, oldBrush);
    DeleteObject(myBrush);
}

void AddBullet::Release()
{
}

Obj* AddBullet::UseItem()
{
    dynamic_cast<Player*> (m_pTarget)->Set_Bullet_Level(BULLET_TWO); // 열거체 조심

    return nullptr;
}
