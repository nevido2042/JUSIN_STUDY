#include "pch.h"
#include "JumpingMonster.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"

CJumpingMonster::CJumpingMonster()
	: /*m_fAccel(0.f),*/ m_fGravity(0.f), /*m_fFallSpeed(0.f),*/ /*m_bJump(false),*/
	/*m_fJumpSpeed(0.f), m_fTime(0.f),*/ m_ullJumpTime(0), m_fPrevY(0.f),
	m_pPlayer(nullptr)
{
}

CJumpingMonster::~CJumpingMonster()
{
	Release();
}

void CJumpingMonster::Initialize()
{
	
	
	m_iHp = 10;
	m_tInfo.fCX = 70.f;
	m_tInfo.fCY = 80.f;
	m_fSpeed = 3.f;

	//m_fAccel = 0.1f;
	m_fGravity = 9.f;
	//m_fFallSpeed = m_fGravity;
	//m_fJumpSpeed = 15.f;

	m_ullJumpTime = GetTickCount64();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_ice_all.bmp", L"EnemyAll");
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
}

void CJumpingMonster::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"EnemyAll");

	GdiTransparentBlt(hDC,            // ���� ���� DC
		m_tRect.left + iScrollX,    // ���� ���� ��ġ ��ǥ X, Y    
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,            // ���� ���� �̹����� ����, ����    ����Ʈ - ����Ʈ �ؼ� ���̸� �־�����ϴϱ�
		(int)m_tInfo.fCY,
		hMemDC,                        // ������ �̹��� DC    
		748,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		23,
		(int)m_tInfo.fCX,            // ������ �̹����� ����, ����
		(int)m_tInfo.fCY,
		RGB(128, 0, 128));
}

int CJumpingMonster::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Jumping();
	//Fall();

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CJumpingMonster::Late_Update()
{
	if (m_ullJumpTime + 1000.f < GetTickCount64())
	{
		m_ullJumpTime = GetTickCount64();
		m_bJump = true;

		m_fJumpPower = 15.f;
	}

	//Jumping();
}

void CJumpingMonster::Release()
{
}

void CJumpingMonster::OnCollision(CObj* _pOther, OBJID _eOtherID)
{
	if (_eOtherID == OBJ_BLOCK)
	{
		Update_Rect();
	}
}

void CJumpingMonster::Fall()
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

void CJumpingMonster::Jumping()
{
	//if (m_bJump)
	//{
	//	//�÷��̾��� ��ġ�� Ȯ���ؼ� �·� ���� ��� ���� �Ǵ�
	//	if (m_pPlayer->Get_Info().fX < Get_Info().fX)
	//	{
	//		m_tInfo.fX -= cosf(45.f * PI / 180.f) * m_fTime;
	//	}
	//	else
	//	{
	//		m_tInfo.fX += cosf(45.f * PI / 180.f) * m_fTime;
	//	}


	//	m_tInfo.fY -= (m_k* sinf(90.f) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
	//	m_fTime += 0.1f;

	//	//�� �������� ���̿� ���� ���̸� ��
	//	//�������� ������ ���� ����
	//	if (m_fPrevY < m_tInfo.fY)
	//	{
	//		m_bJump = false;
	//		m_fTime = 0.f;
	//	}

	//	m_fPrevY = m_tInfo.fY;
	//	Update_Rect();
	//}
}

void CJumpingMonster::Jump_Pattern()
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

	m_fPrevY = m_tInfo.fY;
	m_tInfo.fY -= (m_fJumpPower * sinf(90.f) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
	m_fTime += 0.1f;

}
