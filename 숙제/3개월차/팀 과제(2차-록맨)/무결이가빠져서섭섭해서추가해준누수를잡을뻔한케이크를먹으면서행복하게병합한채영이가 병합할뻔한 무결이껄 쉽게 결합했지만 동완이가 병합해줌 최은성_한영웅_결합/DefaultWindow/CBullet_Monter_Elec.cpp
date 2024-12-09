#include "pch.h"
#include "CBullet_Monter_Elec.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CBullet_Monter_Elec::CBullet_Monter_Elec(): iBulletCount(0)
{
	m_eBulletId = BUL_ELEC_SMALL;
	ZeroMemory(&m_tAttack1, sizeof(IMAGE));
	ZeroMemory(&m_tAttack2, sizeof(IMAGE));
	ZeroMemory(&m_tNowState, sizeof(IMAGE));
}

CBullet_Monter_Elec::~CBullet_Monter_Elec()
{

}

void CBullet_Monter_Elec::Initialize()
{
	m_tInfo.fCX = 50;
	m_tInfo.fCY = 15;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Enemy");

	m_tAttack1 = { 882, 207 };
	m_tAttack2 = { 882, 236 };

	m_fSpeed = 3.0f;
}

int CBullet_Monter_Elec::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBullet_Monter_Elec::Late_Update()
{
	iBulletCount++;

	if (iBulletCount < 3)
	{
		m_tNowState = m_tAttack1;
	}
	else if (iBulletCount < 300)
	{
		m_tNowState = m_tAttack2;
		iBulletCount = 0;
	}
	
}

void CBullet_Monter_Elec::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Enemy");

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,						// ���� ���� DC
		m_tInfo.fX + iScrollX,
		m_tInfo.fY + iScrollY,					// ���� ���� ��ġ ��ǥ X, Y			
		m_tInfo.fCX, m_tInfo.fCY,				// ���� ���� �̹����� ����, ����
		hMemDC,									// ������ �̹��� DC	
		m_tNowState._iX, m_tNowState._iY,		// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		m_tInfo.fCX, m_tInfo.fCY,				// ������ �̹����� ����, ����
		RGB(128, 0, 128));						// ��� ȿ�� ����(�״�� ���)

}

void CBullet_Monter_Elec::Release()
{
}
