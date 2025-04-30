#include "pch.h"
#include "CObj.h"

CObj::CObj() : m_fSpeed(0.f)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	D3DXMatrixIdentity(&m_tInfo.matWorld);

}

CObj::~CObj()
{
}

