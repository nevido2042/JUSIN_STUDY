#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CLineMgr.h"

CPlayer::CPlayer() 
: m_bJump(false), m_fJumpPower(0.f), m_fTime(0.f)

{
	ZeroMemory(&m_tPosin, sizeof(POINT));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo  = { 100.f, WINCY / 2.f, 100.f, 100.f };
	m_fSpeed = 10.f;
	m_fDistance = 100.f;

	m_fJumpPower = 20.f;
}

int CPlayer::Update()
{
	Key_Input();

	// CObj::Update_Rect();
	__super::Update_Rect();

	return OBJ_NOEVENT;
}
void CPlayer::Late_Update()
{
	Jumping();

	// degree to radian

	// m_tPosin.x = long(m_tInfo.fX + (m_fDistance * cosf(m_fAngle * (PI / 180.f))));
	// m_tPosin.y = long(m_tInfo.fY - (m_fDistance * sinf(m_fAngle * (PI / 180.f))));
}


void CPlayer::Render(HDC hDC)
{
	Rectangle(hDC, 
		m_tRect.left, 
		m_tRect.top, 
		m_tRect.right, 
		m_tRect.bottom);

	// Æ÷½Å
	// MoveToEx(hDC, (int)m_tInfo.fX, (int)m_tInfo.fY, nullptr);
	// LineTo(hDC, m_tPosin.x, m_tPosin.y);
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()
{
	// if (GetAsyncKeyState(VK_UP))
	// {
	// 	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	// 	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));
	// }
	// 
	// if (GetAsyncKeyState(VK_DOWN))
	// {
	// 	m_tInfo.fX += -m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	// 	m_tInfo.fY -= -m_fSpeed * sinf(m_fAngle * (PI / 180.f));
	// }

	// if (GetAsyncKeyState(VK_SPACE))
	// {
	// 	CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CGuideBullet>::
	// 		Create((float)m_tPosin.x, (float)m_tPosin.y, m_fAngle));
	// }
	// 
	// if (GetAsyncKeyState(VK_F1))
	// {
	// 	CObjMgr::Get_Instance()->Add_Object(OBJ_SHIELD, Create_Shield());
	// }
	
	float	fY(0.f);

	if (GetAsyncKeyState(VK_LEFT))
	{
		m_tInfo.fX -= m_fSpeed;

		//if (CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY))
		//	m_tInfo.fY = fY;
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		m_tInfo.fX += m_fSpeed;

		// if (CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY))
		// 	m_tInfo.fY = fY;
	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		m_bJump = true;
	}

}

void CPlayer::Jumping()
{
	float	fY(0.f);

	bool	bLineCol = CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY);

	if (m_bJump)
	{
		m_tInfo.fY -= (m_fJumpPower * sinf(45.f) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
		m_fTime += 0.2f;

		if (bLineCol && (fY < m_tInfo.fY))
		{
			m_bJump = false;
			m_fTime = 0.f;
			m_tInfo.fY = fY;
		}
	}
	else if (bLineCol)
	{
		m_tInfo.fY = fY;
	}
}

CObj* CPlayer::Create_Shield()
{
	CObj* pShield = CAbstractFactory<CShield>::Create();
	
	pShield->Set_Target(this);

	return pShield;
}


