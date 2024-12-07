#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"

CPlayer::CPlayer()
	: m_bJump(false), m_fJumpPower(0.f), m_fTime(0.f)

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

			if (m_ullDamaged_Time + 400 < GetTickCount64())
				Key_Input();

			__super::Update_Rect();
			return OBJ_NOEVENT;
		}
		else
			m_bDamaged = false;
	}

	Key_Input();
	CustomJumping(); // Ŀ���� ���� ����

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

	Rectangle(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top,
		m_tRect.right + iScrollX,
		m_tRect.bottom);

	if (m_bDamaged)
	{
		if (m_bBlink)
		{
			Rectangle(hDC,
				m_tRect.left + iScrollX,
				m_tRect.top + iScrollY,
				m_tRect.right + iScrollX,
				m_tRect.bottom + iScrollY);
		}

		if (m_ullBlink + 50 < GetTickCount64())
		{
			m_bBlink = !m_bBlink;
			m_ullBlink = GetTickCount64();
		}
	}
	else
	{
		Rectangle(hDC,
			m_tRect.left + iScrollX,
			m_tRect.top + iScrollY,
			m_tRect.right + iScrollX,
			m_tRect.bottom + iScrollY);
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
		CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY, m_eDir));
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
		if (m_bisGround) { m_bJump = true; } // ���鿡 ���� ���� ���� ����
		m_fJumpPower = 6.f; // �ʱ� ���� �Ŀ� ����
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('Z'))
	{
		if (!m_bisGround && m_bJump)
		{
			m_fJumpPower += 0.5f; // ���� �Ŀ� ����

			if (m_fJumpPower >= 16.f)
				m_fJumpPower = 16.f; // �ִ� ���� �Ŀ� ����
		}
	}
	if (CKeyMgr::Get_Instance()->Key_Up('X'))
	{
		if (m_ullLast_Bullet + m_iBullet_Cooltime < GetTickCount64()) // �Ѿ� �߻��ֱ�
		{
			Create_Bullet();
			m_ullLast_Bullet = GetTickCount64();
		}
	}
	if (CKeyMgr::Get_Instance()->Key_Up('Z'))
	{
		if (m_bisGround)
		{
			m_fTime = 0.1f; // �ð� �ʱ�ȭ
			m_bJump = false; // ���� ����
		}
	}

}

void CPlayer::Jumping()
{
	float	fY(0.f);

	//�����浹����
	//�����ڵ��  m_tInfo.fY �߰�
	bool bLineCol = CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY, m_tInfo.fY);
	if (m_bJump)
	{
		// ������ ��꿡 ���� y ��ġ ����
		m_tInfo.fY -= (m_fJumpPower * sinf(45.f) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
		m_fTime += 0.2f;

		if (bLineCol && (fY < m_tInfo.fY))
		{
			// �浹 �� ���� ����
			m_bJump = false;
			m_fTime = 0.f;
			m_tInfo.fY = fY;
		}
	}
	else if (bLineCol)
	{
		// ���� �浹 �� ��ġ ����
		m_tInfo.fY = fY;
	}
	else
	{
		// ���߿��� �߷� ����
		m_tInfo.fY += 9.8f * m_fTime * m_fTime * 0.5f;
		m_fTime += 0.2f;
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


	if (0 > m_tInfo.fX + m_tInfo.fCX + iScrollX)
		CScrollMgr::Get_Instance()->SET_DIR(DIR_RIGHT);

	if (WINCX < m_tInfo.fX - m_tInfo.fCX + iScrollX)
		CScrollMgr::Get_Instance()->SET_DIR(DIR_LEFT);

	if (0 > m_tInfo.fY + m_tInfo.fCY + iScrollY)
		CScrollMgr::Get_Instance()->SET_DIR(DIR_DOWN);

	if (WINCY < m_tInfo.fY - m_tInfo.fCY + iScrollY)
		CScrollMgr::Get_Instance()->SET_DIR(DIR_UP);

}

void CPlayer::CustomJumping()
{
	float Ground_Y(0.f);

	if (m_bJump)
	{
		m_bisGround = false;// ���� �߿��� ���鿡�� ������
		float temp = (m_fJumpPower * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
		m_tInfo.fY -= temp;
		m_fTime += 0.1f; // �����ڵ�� �Ŀ� 0.2���� 0.1f �� ������

	}
	else
	{
		if (!m_bisGround) // ���鿡 ������������
		{
			//���߿� ���� ���� �߷�����
			m_tInfo.fY += 9.8f * m_fTime * m_fTime * 0.5f; // �߷�
			m_fTime += 0.1f;
		}
		else
		{
			m_fTime = 0; // ���� ���� �� �ʱ�ȭ
		}
	}
}



