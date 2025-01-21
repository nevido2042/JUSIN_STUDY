#include "pch.h"
#include "CObj.h"

CObj::CObj()
	:m_pTexInfo(nullptr)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
}

CObj::~CObj()
{
}

INFO& CObj::Get_Info()
{
    return m_tInfo;
}

void CObj::Set_Pos(float _fX, float _fY, float _fZ)
{
	m_tInfo.vPos.x = _fX;
	m_tInfo.vPos.y = _fY;
	m_tInfo.vPos.z = _fZ;
}

void CObj::Initialize()
{
}

void CObj::Render(CDevice* _pDevice)
{

}
