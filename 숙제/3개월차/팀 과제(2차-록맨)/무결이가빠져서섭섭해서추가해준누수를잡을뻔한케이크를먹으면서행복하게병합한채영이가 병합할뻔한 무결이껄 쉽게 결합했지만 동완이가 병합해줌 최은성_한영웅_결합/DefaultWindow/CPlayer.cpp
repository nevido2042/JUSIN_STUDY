#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CRolling_Cutter.h"
#include "CRolling_Cutter_P.h"
#include "CWeaponsMenuMgr.h"
#include "CSuper_Arm.h"
#include "CAnimMgr.h"



CPlayer::CPlayer()
{
	ZeroMemory(&m_tPosin, sizeof(POINT));
	m_iScroll_Move = 0;
	m_bDamaged = 0;
	m_ullDamaged_Time = 0;
	m_ullBlink = 0;
	m_iBullet_Cooltime = 0;
	m_ullLast_Bullet = 0;
	m_iHp = 0;
	m_eBullet_ID = BUL_NORMAL;
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo = { 100.f, WINCY / 2.f, 42.f, 48.f };//�ʱ���ġ
	m_fSpeed = 10.f;//�̵��ӵ�
	m_fDistance = 100.f;//�̵��Ÿ�
	m_fJumpPower = 3.f; //�⺻ ���� �Ŀ�
	m_iHp = 100; // �÷��̾� HP(����)
	m_iBullet_Cooltime = 200; // �Ѿ� �߻��ֱ� ( ���� )
	m_eDir = DIR_RIGHT; // �����ִ¹��� ( ���� )

	for (int i = 1; i < BUL_END; ++i)
	{
		m_iBulletCount[i] = -1;	
	}
	m_iBulletCount[BUL_NORMAL] = 10;
	m_iBulletCount[BUL_ICE] = 10;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/player_all_2x.bmp", L"Player");
	Initialize_Animation();
}


void CPlayer::Initialize_Animation()
{

	// Right.LT.X = IMAGE_SIZE.X - left.LT.X - FRAME_SIZE.X
	// Left ����
	// IMAGE_SIZE = 644

	FPOINT t_size = { 48, 68 };

	//run
	ANIMATION_CREATE(L"player_run_left", this, t_size, 130, 4, L"Player_Left");
	ANIMATION_EDIT(L"player_run_left", 124, 0, t_size, 0);
	ANIMATION_EDIT(L"player_run_left", 124 + 48, 0, t_size, 1);
	ANIMATION_EDIT(L"player_run_left", 124 + 48 * 2, 0, t_size, 2);
	ANIMATION_EDIT(L"player_run_left", 124 + 48, 0, t_size, 3);
	ANIMATION_CREATE(L"player_run_right", this, t_size, 130, 4, L"Player_Right");
	ANIMATION_EDIT(L"player_run_right", 472, 0, t_size, 0);
	ANIMATION_EDIT(L"player_run_right", 472 - 48, 0, t_size, 1);
	ANIMATION_EDIT(L"player_run_right", 472 - 48 * 2, 0, t_size, 2);
	ANIMATION_EDIT(L"player_run_right", 472 - 48, 0, t_size, 3);

	//idle
	ANIMATION_CREATE(L"player_idle_right", this, t_size, 500, 1, L"Player_Right");
	ANIMATION_EDIT(L"player_idle_right", 203, 0, t_size, 0);
	ANIMATION_CREATE(L"player_idle_left", this, t_size, 500, 1, L"Player_Left");
	ANIMATION_EDIT(L"player_idle_left", 393, 0, t_size, 0);

	//jump
	int iImgSize = 644;
	t_size = { 52, 68 };
	ANIMATION_CREATE(L"player_jump_left", this, t_size, 500, 1, L"Player_Left");
	ANIMATION_EDIT(L"player_jump_left", 62, 0, t_size, 0);

	ANIMATION_CREATE(L"player_jump_right"	// �ִϸ��̼��� Ű ��(map.first)
		, this								// this ����(Get_Info �뵵)
		, t_size							// �̹��� �� ĭ�� ������
		, 500								// ������ �� �ð�(ms, 50 ~ 150 ��õ)
		, 1									// ������ ��
		, L"Player_Right");					// BmpMgr::Insert_Bmp�� ���� �� ������ ���� �̹����� Ű ��

	ANIMATION_EDIT(L"player_jump_right"		// �ִϸ��̼��� Ű ��(map.first)
		, iImgSize - 62 - t_size.fX			// �� �������� LT.X ��ǥ
		// �¿������ (��ü ���α��� - �ݴ����� LT.X��ǥ - �̹����� ���α���) �� �ᵵ �ǰ� �׳� ����ؼ� �־..
		, 0									// �� �������� LT.Y ��ǥ
		, t_size							// �̹��� �� ĭ�� ������
		, 0);								// ���° ���������� (�ε����̹Ƿ� 0���� ����)

	//damaged 96 83
	ANIMATION_CREATE(L"player_damaged_right", this, t_size, 500, 1, L"Player_Right");
	ANIMATION_EDIT(L"player_damaged_right", 644 - 96 - 52, 83, t_size, 0);
	ANIMATION_CREATE(L"player_damaged_left", this, t_size, 500, 1, L"Player_Left");
	ANIMATION_EDIT(L"player_damaged_left", 96, 83, t_size, 0);
}
int CPlayer::Update()
{
	if (m_iHp <= 0) // �÷��̾� �� ������
		return OBJ_DEAD;

	if (m_bDamaged) // �ǰݽ�
	{
		if (m_ullDamaged_Time + 500 > GetTickCount64())
		{
			switch (m_eDir)// �ǰ� ���� ����
			{
			case DIR_LEFT:
				m_tInfo.fX += 1.f;
				break;
			case DIR_RIGHT:
				m_tInfo.fX -= 1.f;
				break;
			default:
				break;
			}

			if (m_ullDamaged_Time + 200 < GetTickCount64())
				Key_Input();

			__super::Update_Rect();
			return OBJ_NOEVENT;
		}
		else
			m_bDamaged = false;
	}

	Key_Input();
	//CustomJumping(); // Ŀ���� ���� ����
	Jumping();


	// �浹 �ڽ� ����
	__super::Update_Rect();

	return OBJ_NOEVENT;
}
void CPlayer::Late_Update()
{
	if (m_bDamaged)
		return;

	m_CollisionRect = { m_tRect.left, m_tRect.bottom, m_tRect.right, m_tRect.bottom + 5 };
	Offset();// ȭ�� ��ũ�� ó��

}

void CPlayer::Render(HDC hDC)
{
	//��ũ�� ��ġ ���
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	;

	switch (m_eDir)
	{
	case DIR_LEFT:
		if (m_bDamaged)
		{
			if (m_bBlink)
				ANIMATION_RENDER(hDC, L"player_damaged_left", this);
			if (m_ullBlink + 50 < GetTickCount64())
			{
				m_bBlink = !m_bBlink;
				m_ullBlink = GetTickCount64();
			}
		}
		else if (GetAsyncKeyState('Z'))
			ANIMATION_RENDER(hDC, L"player_jump_left", this);
		else if (GetAsyncKeyState(VK_LEFT))
			ANIMATION_RENDER(hDC, L"player_run_left", this);
		else
			ANIMATION_RENDER(hDC, L"player_idle_left", this);
		break;
	case DIR_RIGHT:
		if (m_bDamaged)
		{
			if (m_bBlink)
				ANIMATION_RENDER(hDC, L"player_damaged_right", this);
			if (m_ullBlink + 50 < GetTickCount64())
			{
				m_bBlink = !m_bBlink;
				m_ullBlink = GetTickCount64();
			}
		}
		else if (GetAsyncKeyState('Z'))
			ANIMATION_RENDER(hDC, L"player_jump_right", this);
		else if (GetAsyncKeyState(VK_RIGHT))
			ANIMATION_RENDER(hDC, L"player_run_right", this);
		else
			ANIMATION_RENDER(hDC, L"player_idle_right", this);

		break;
	}


	TCHAR szBuf[32] = {};
	wsprintf(szBuf, L"�÷��̾� ��ǥ : (%d,%d)", (int)m_tInfo.fX, (int)m_tInfo.fY);
	TextOut(hDC, 0, 0, szBuf, lstrlen(szBuf));

	wsprintf(szBuf, L"��ũ�� : (%d,%d)", iScrollX, iScrollY);
	TextOut(hDC, 300, 0, szBuf, lstrlen(szBuf));

	wsprintf(szBuf, L"HP : (%d,%d)", m_iHp, 100);
	TextOut(hDC, 0, 30, szBuf, lstrlen(szBuf));
}

void CPlayer::Release()
{
}

void CPlayer::Create_Bullet()
{
	switch (m_eBullet_ID) // �渴 Ÿ�� Ȯ�� // ���߿� �����ϰ� �ϱ�
	{
	case BUL_NORMAL:
		CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CRolling_Cutter_P>::Create(m_tInfo.fX, m_tInfo.fY, m_eDir, this, nullptr));
		break;
	default:
		break;
	}

}

void CPlayer::Set_Damaged(const DIRECTION& _eDir)
{
	if (m_bDamaged) //������ 2�����ö�
		return;

	m_fJumpPower = 0.f;
	m_bDamaged = true;;
	m_ullDamaged_Time = m_ullBlink = GetTickCount64();
	m_eDir = _eDir;
	m_iHp -= 10;
}

void CPlayer::Set_Fallen(const DIRECTION& _eDir)
{
	if (m_bDamaged) //������ 2�����ö�
		return;

	m_fJumpPower = 0.f;
	m_bDamaged = true;;
	m_ullDamaged_Time = m_ullBlink = GetTickCount64();
	m_eDir = _eDir;
}

void CPlayer::Key_Input()
{
	float	fY(0.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_eDir = DIR_LEFT;
		m_tInfo.fX -= m_fSpeed; // �������� �̵�
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		m_eDir = DIR_RIGHT;
		m_tInfo.fX += m_fSpeed; // ���������� �̵�
	}


	if (CKeyMgr::Get_Instance()->Key_Down('Z'))
	{
		if (!m_bJump) // ���鿡 ���� ���� ���� ����
		{
			m_bJump = true;
			m_fJumpPower = 6.f; // �ʱ� ���� �Ŀ� ����
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('Z'))
	{

		m_fJumpPower += 0.5f; // ���� �Ŀ� ����

		if (m_bJump)
		{
			m_fJumpPower += 0.5f; // ���� �Ŀ� ����

			if (m_fJumpPower >= 16.f)
				m_fJumpPower = 16.f; // �ִ� ���� �Ŀ� ����
		}
	}

	//if (CKeyMgr::Get_Instance()->Key_Up('Z'))
	//{
	//	if (m_bisGround)
	//	{
	//		m_fTime = 0.1f; // �ð� �ʱ�ȭ
	//		m_bJump = false; // ���� ����
	//	}
	//}
	if (CKeyMgr::Get_Instance()->Key_Pressing('X'))
	{
		if (m_ullLast_Bullet + m_iBullet_Cooltime < GetTickCount64()) // �Ѿ� �߻��ֱ�
		{
			Create_Bullet();
			m_ullLast_Bullet = GetTickCount64();
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CWeaponsMenuMgr::Get_Instance()->Open_Menu(m_iBulletCount);
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

		//int iOffSetminX = 100; // ������ �������� ����� �ڵ� 
		//int iOffSetmaxX = 700; //

	int		iOffSetminX = 0;
	int		iOffSetmaxX = WINCX;
	int		iOffSetminY = 0;
	int		iOffSetmaxY = WINCY;

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//�����ڵ�
	//if (iOffSetminX > m_tInfo.fX + iScrollX)
		//CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed); // ȭ�� ���� �̵�

	//if (iOffSetmaxX < m_tInfo.fX + iScrollX)
		//CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed); // ȭ�� ������ �̵�


	if (0 > m_tInfo.fX + m_tInfo.fCX * 0.5f + iScrollX)
		CScrollMgr::Get_Instance()->SET_DIR(DIR_RIGHT);

	if (WINCX < m_tInfo.fX - m_tInfo.fCX * 0.5f + iScrollX)
		CScrollMgr::Get_Instance()->SET_DIR(DIR_LEFT);

	if (0 > m_tInfo.fY + m_tInfo.fCY * 0.5f + iScrollY)
		CScrollMgr::Get_Instance()->SET_DIR(DIR_DOWN);

	if (WINCY < m_tInfo.fY - m_tInfo.fCY * 0.5f + iScrollY)
		CScrollMgr::Get_Instance()->SET_DIR(DIR_UP);

}

//void CPlayer::CustomJumping()
//{
//	float Ground_Y(0.f);
//
//	if (m_bJump)
//	{
//		m_bisGround = false;// ���� �߿��� ���鿡�� ������
//		float temp = (m_fJumpPower * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
//		m_tInfo.fY -= temp;
//		m_fTime += 0.1f; // �����ڵ�� �Ŀ� 0.2���� 0.1f �� ������
//
//	}
//	else
//	{
//		if (!m_bisGround) // ���鿡 ������������
//		{
//			//���߿� ���� ���� �߷�����
//			m_tInfo.fY += 9.8f * m_fTime * m_fTime * 0.5f; // �߷�
//			m_fTime += 0.1f;
//		}
//		else
//		{
//			m_fTime = 0; // ���� ���� �� �ʱ�ȭ
//		}
//	}
//}