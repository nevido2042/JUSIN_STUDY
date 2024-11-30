#include "pch.h"
#include "AddSatellite.h"
#include "Satellite.h"

AddSatellite::AddSatellite()
{
}

AddSatellite::~AddSatellite()
{
}

void AddSatellite::Initialize()
{
    
    m_tInfo.fCX = 20.f;
    m_tInfo.fCY = 20.f;
    m_iHp = 1;
}

int AddSatellite::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    Obj::Update_Rect();
    return OBJ_NOEVENT;
 
}

void AddSatellite::Late_Update()
{

}

void AddSatellite::Render(HDC _hdc)
{
    HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, myBrush);
    Ellipse(_hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
    SelectObject(_hdc, oldBrush);
    DeleteObject(myBrush);
}

void AddSatellite::Release()
{
}

Obj* AddSatellite::UseItem()
{
    Obj* pMoon = new Satellite;
    pMoon->Initialize();
    

    return pMoon;
}
