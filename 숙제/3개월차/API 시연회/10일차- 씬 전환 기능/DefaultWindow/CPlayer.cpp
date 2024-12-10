#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

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
	m_tInfo  = { 100.f, WINCY / 2.f, 60.f, 60.f };
	m_fSpeed = 10.f;
	m_fDistance = 100.f;

	m_fJumpPower = 20.f;


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/maja2.bmp", L"Player");
}

int CPlayer::Update()
{
	Key_Input();

	__super::Update_Rect();

	return OBJ_NOEVENT;
}
void CPlayer::Late_Update()
{
	Jumping();
	Offset();	
}


void CPlayer::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player");

	// BitBlt(hDC,						// ���� ���� DC
	// 	m_tRect.left + iScrollX,	// ���� ���� ��ġ ��ǥ X, Y	
	// 	m_tRect.top,
	// 	(int)m_tInfo.fCX,			// ���� ���� �̹����� ����, ����
	// 	(int)m_tInfo.fCY,
	// 	hMemDC,						// ������ �̹��� DC
	// 	0,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
	// 	0,
	// 	SRCCOPY);					// ��� ȿ�� ����(�״�� ���)


	GdiTransparentBlt(hDC,			// ���� ���� DC
		m_tRect.left + iScrollX,	// ���� ���� ��ġ ��ǥ X, Y	
		m_tRect.top,
		(int)m_tInfo.fCX,			// ���� ���� �̹����� ����, ����
		(int)m_tInfo.fCY,
		hMemDC,						// ������ �̹��� DC	
		0,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		0,
		(int)m_tInfo.fCX,			// ������ �̹����� ����, ����
		(int)m_tInfo.fCY,
		RGB(255, 255, 255));		// ������ ����
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()
{
	float	fY(0.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_tInfo.fX -= m_fSpeed;
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		m_tInfo.fX += m_fSpeed;
	}

	if (CKeyMgr::Get_Instance()->Key_Up(VK_SPACE))
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

void CPlayer::Offset()
{
	/*int		iOffSetX = WINCX >> 1;

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	if (iOffSetX > m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);

	if (iOffSetX < m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed);*/

	int		iOffSetminX = 100;
	int		iOffSetmaxX = 700;

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	if (iOffSetminX > m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);

	if (iOffSetmaxX < m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed);
}

CObj* CPlayer::Create_Shield()
{
	CObj* pShield = CAbstractFactory<CShield>::Create();
	
	pShield->Set_Target(this);

	return pShield;
}


