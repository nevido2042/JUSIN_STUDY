#include "pch.h"
#include "CObj.h"

CObj::CObj() 
	: m_fSpeed(0.f), m_eDir(DIR_END), m_bDead(false),
	m_fAngle(0.f), m_fDistance(0.f), m_pTarget(nullptr),
	m_eOBJID(OBJ_END),m_bJump(false), m_fJumpPower(0.f), m_fTime(0.f)

{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
}

CObj::~CObj()
{
}

void CObj::Update_Rect()
{
	m_tRect.left	= LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top		= LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right	= LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom	= LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CObj::Jumping()
{
	if (m_bJump)
	{
		Jump_Pattern();
		if (m_fCellingY != 0 && (m_fCellingY > m_tInfo.fY))
		{
			m_tInfo.fY = m_fCellingY;
			m_fJumpPower = 0.f;
			m_fTime *= 0.5f;
		}

		if (m_fLineY != 0 && (m_fLineY < m_tInfo.fY))
		{
			m_bJump = false;
			m_fTime = 0.f;
			m_tInfo.fY = m_fLineY;
		}
	}
	else if (m_fLineY == 0.f || m_fLineY > m_tInfo.fY)
	{
		m_bJump = true;
		m_fJumpPower = 0.f;
	}
	else
	{
		m_tInfo.fY = m_fLineY;
	}
}

void CObj::Jump_Pattern()
{
	
	m_tInfo.fY -= (m_fJumpPower * m_fTime) - (G_FORCE * SQUARE(m_fTime)) * 0.5f;
	m_fTime += 0.2f;


}
