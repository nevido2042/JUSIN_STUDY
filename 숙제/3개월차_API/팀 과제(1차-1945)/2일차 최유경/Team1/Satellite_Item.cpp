#include "pch.h"
#include "Satellite_Item.h"
#include "Satellite.h"

Satellite_Item::Satellite_Item()
	:m_pSatelliteList(nullptr)
{
}

Satellite_Item::~Satellite_Item()
{
}

void Satellite_Item::Upgrade_Target()
{
	Satellite* pSatellite = new Satellite;
	pSatellite->Initialize();
	pSatellite->Set_Target(m_pTarget);
	m_pSatelliteList->push_back(pSatellite);
}

void Satellite_Item::Render(HDC _hDC)
{
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 255));
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);
	Rectangle(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	SelectObject(_hDC, oldBrush);
	DeleteObject(myBrush);
}
