#include "pch.h"
#include "Portion.h"

Portion::Portion()
{
}

Portion::~Portion()
{
    Release();
}

void Portion::Initialize()
{
 
    m_tInfo.fCX = 20.f;
    m_tInfo.fCY = 20.f;
    m_iHp = 1;
}

int Portion::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    Obj::Update_Rect();
    return OBJ_NOEVENT;
}

void Portion::Late_Update()
{
}

void Portion::Render(HDC _hdc)
{
    
    HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(100, 20, 60));
    HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, myBrush);
    Ellipse(_hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
    SelectObject(_hdc, oldBrush);
    DeleteObject(myBrush);
}

void Portion::Release()
{
}

Obj* Portion::UseItem()
{
    m_pTarget->Set_Hp(m_pTarget->Get_Hp() + 15);

    return nullptr;
}
