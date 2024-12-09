#include "pch.h"
#include "Boss_IceMan.h"
#include "CScrollMgr.h"
#include "CIce_Slasher.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"

CBoss_IceMan::CBoss_IceMan()
	: /*m_fAccel(0.f),*/ m_fGravity(0.f), /*m_fFallSpeed(0.f),*/ m_bJump(false),
	m_fJumpSpeed(0.f), m_fTime(0.f), m_ullJumpTime(0), m_fPrevY(0.f),
	m_ullFireTime(0), m_pPlayer(nullptr)
{
}

CBoss_IceMan::~CBoss_IceMan()
{
	Release();
}

void CBoss_IceMan::Initialize()
{
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;
	m_fSpeed = 3.f;

	//m_fAccel = 0.1f;
	m_fGravity = 2.f;
	//m_fFallSpeed = m_fGravity;
	m_fJumpSpeed = 15.f;
	m_ullJumpTime = GetTickCount64();
	m_ullFireTime = GetTickCount64();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_ice_all.bmp", L"BossIceAll");

	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
}

int CBoss_IceMan::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Fall();

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBoss_IceMan::Late_Update()
{
	if (m_ullJumpTime + 100.f < GetTickCount64())
	{
		m_ullJumpTime = GetTickCount64();
		m_bJump = true;
	}

	if (m_ullFireTime + 500.f < GetTickCount64())
	{
		m_ullFireTime = GetTickCount64();
		Fire();
	}

	Jumping();
}

void CBoss_IceMan::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BossIceAll");

	GdiTransparentBlt(hDC,            // ���� ���� DC
		m_tRect.left + iScrollX,    // ���� ���� ��ġ ��ǥ X, Y    
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,            // ���� ���� �̹����� ����, ����    ����Ʈ - ����Ʈ �ؼ� ���̸� �־�����ϴϱ�
		(int)m_tInfo.fCY,
		hMemDC,                        // ������ �̹��� DC    
		7,                            // ��Ʈ�� ��� ���� ��ǥ(Left, top)
		14,
		(int)m_tInfo.fCX,            // ������ �̹����� ����, ����
		(int)m_tInfo.fCY,
		RGB(128, 0, 128));            // ������ ����
}

void CBoss_IceMan::Release()
{
}

void CBoss_IceMan::OnCollision(CObj* _pOther, OBJID _eOtherID)
{
	if (_eOtherID == OBJ_BLOCK)
	{
		Update_Rect();
	}

}

void CBoss_IceMan::Fall()
{
	if (!m_bJump)
	{
		//m_fFallSpeed += m_fAccel;
		Set_PosY(m_fGravity);

		if (m_bGround)
		{
			m_fGravity;
		}
	}

}

void CBoss_IceMan::Jumping()
{
	if (m_bJump)
	{
		//�÷��̾��� ��ġ�� Ȯ���ؼ� �·� ���� ��� ���� �Ǵ�
		if (m_pPlayer->Get_Info().fX < Get_Info().fX)
		{
			m_tInfo.fX -= cosf(45.f * PI / 180.f) * m_fTime;
		}
		else
		{
			m_tInfo.fX += cosf(45.f * PI / 180.f) * m_fTime;
		}

		
		m_tInfo.fY -= (m_fJumpSpeed * sinf(90.f) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
		m_fTime += 0.1f;

		//�� �������� ���̿� ���� ���̸� ��
		//�������� ������ ���� ����
		if (m_fPrevY < m_tInfo.fY)
		{
			m_bJump = false;
			m_fTime = 0.f;
		}

		m_fPrevY = m_tInfo.fY;
		Update_Rect();
	}
}

void CBoss_IceMan::Fire()
{
	if (m_pPlayer->Get_Info().fX < Get_Info().fX)
	{
		CObjMgr::Get_Instance()->Add_Object(
			OBJ_BOSSBULLET,
			CAbstractFactory<CIce_Slasher>::Create(m_tInfo.fX, m_tInfo.fY, DIR_LEFT));
	}
	else
	{
		CObjMgr::Get_Instance()->Add_Object(
			OBJ_BOSSBULLET,
			CAbstractFactory<CIce_Slasher>::Create(m_tInfo.fX, m_tInfo.fY, DIR_RIGHT));
	}


}
