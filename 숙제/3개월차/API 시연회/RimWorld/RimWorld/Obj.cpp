#include "pch.h"
#include "Obj.h"

CObj::CObj() : m_fSpeed(0.f), m_eDir(DIR_END), m_bDestroyed(false),
m_fAngle(0.f), m_fDistance(0.f), m_pTarget(nullptr), m_pImgKey(nullptr),
m_iDrawID(0), m_eRenderID(RENDER_END)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CObj::~CObj()
{
	//Release();
}

void CObj::OnCollision(OBJID _eID, CObj* _pOther)
{
}

void CObj::Update_Rect()
{
	m_tRect.left	= LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top		= LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right	= LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom	= LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CObj::Move_Frame()
{
	if (m_tFrame.ullTime + m_tFrame.ullSpeed < GetTickCount64())
	{
		++m_tFrame.iFrameStart;

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.ullTime = GetTickCount64();
	}
}
