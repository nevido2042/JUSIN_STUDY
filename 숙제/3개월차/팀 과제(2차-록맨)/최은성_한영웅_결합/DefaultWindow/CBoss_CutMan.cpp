#include "pch.h"
#include "CBoss_CutMan.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CRolling_Cutter.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"


CBoss_CutMan::CBoss_CutMan() : m_bJump(false), m_bisGround(false), m_iImagePose(0), m_iAnimationX(0),
m_iAnimationY(0), m_fJumpPower(), m_fTime(), m_eAction(NONE), m_bShoot(false), m_bRunAni(true)
{
}

CBoss_CutMan::~CBoss_CutMan()
{
}

void CBoss_CutMan::Initialize()
{
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;


	m_tAnimationSize = {0,0};
	m_fJumpPower = 10.f;

	m_fSpeed = 3.f;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_cut_all.bmp", L"CutMan");
}

int CBoss_CutMan::Update()
{
	if(m_bDead)
		return OBJ_DEAD;

	m_ullCerrentTime = GetTickCount64();
	if (m_eAction == NONE && m_bisGround)
	{
		Select_Action();
	}
	else
	{
		switch (m_eAction)
		{
		case RUN:
			Action_Run();
			break;
		case SHOOT:
			Action_Shoot();
			break;
		case IDLE:
			Action_Idle();
			break;
		default:
			break;
		}
	}

	Update_Animation();
	//Custom_Jumping(); // Ŀ���� ���� ����
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBoss_CutMan::Late_Update() // �浹ó�� �������� �̷�� �ϴ� �ھƵ�
{
	m_tInfo.fY = 495;
	m_bisGround = true;
}

void CBoss_CutMan::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CutMan");

	GdiTransparentBlt(hDC,            // ���� ���� DC
		m_tRect.left + iScrollX - m_tAnimationSize.x,    // ���� ���� ��ġ ��ǥ X, Y    
		m_tRect.top + iScrollY - m_tAnimationSize.y,
		(int)m_tInfo.fCX + m_tAnimationSize.x,            // ���� ���� �̹����� ����, ����
		(int)m_tInfo.fCY + m_tAnimationSize.y,
		hMemDC,                        // ������ �̹��� DC    
		m_iAnimationX - m_tAnimationSize.x,                            // ��Ʈ�� ��� ���� ��ǥ(Left, top)
		m_iAnimationY - m_tAnimationSize.y,
		(int)m_tInfo.fCX + m_tAnimationSize.x,            // ������ �̹����� ����, ����
		(int)m_tInfo.fCY + m_tAnimationSize.y,
		RGB(128, 0, 128));

	Action_Render(hDC);
}

void CBoss_CutMan::Release()
{
}
void CBoss_CutMan::Update_Animation()
{
	switch (m_iImagePose)
	{
	case 0:
		m_iAnimationX = 10;
		m_iAnimationY = 26;
		m_tInfo.fCY = 48.f;
		break;
	case 1:
		m_iAnimationX = 80;
		m_iAnimationY = 26;
		m_tInfo.fCY = 48.f;
		break;
	case 3:
		m_iAnimationX = 230;
		m_iAnimationY = 32;
		m_tInfo.fCY = 42.f;
		break;
	case 4:
		m_iAnimationX = 297;
		m_iAnimationY = 28;
		m_tInfo.fCY = 46.f;
		break;
	case 5:
		m_iAnimationX = 355;
		m_iAnimationY = 32;
		m_tInfo.fCY = 42.f;
		break;
	case 6:
		m_iAnimationX = 413;
		m_iAnimationY = 26;
		m_tInfo.fCY = 48.f;
		break;
	case 7:
		m_iAnimationX = 497;
		m_iAnimationY = 26;
		m_tInfo.fCY = 48.f;
		break;
	}

	if (m_bShoot)
		m_tAnimationSize.y = 0;
	else
		m_tAnimationSize.y = 17;

	if (m_iImagePose == 7)
		m_tAnimationSize.x = 14;
	else
		m_tAnimationSize.x = 0;
}

void CBoss_CutMan::Custom_Jumping()
{
	float Ground_Y(0.f);

	if (m_bJump)
	{
		m_bisGround = false;// ���� �߿��� ���鿡�� ������
		float temp = (m_fJumpPower * m_fTime) - (G_FORCE * SQUARE(m_fTime)) * 0.5f;
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

void CBoss_CutMan::Select_Action()
{
	m_eAction = (CUTMAN_ACTION)(rand() % 3+1);
	m_ullActionTimer = m_ullAnimationFrame = m_ullCerrentTime;
}

void CBoss_CutMan::Action_Run()
{
	if (m_ullActionTimer + 1000 > m_ullCerrentTime)
	{
		if (m_iImagePose < 3 || m_iImagePose > 5)
			m_iImagePose = 3;

		if (m_pTarget->Get_Info().fX > m_tInfo.fX)
		{
			m_tInfo.fX += m_fSpeed;
		}
		else
		{
			m_tInfo.fX -= m_fSpeed;
		}
		if (m_ullAnimationFrame + 150 < m_ullCerrentTime)
		{
			if (m_bRunAni)
			{
				m_iImagePose++;
				if (m_iImagePose >= 5)
					m_bRunAni = !m_bRunAni;
			}
			else
			{
				m_iImagePose--;
				if (m_iImagePose <= 3)
					m_bRunAni = !m_bRunAni;
			}
			m_ullAnimationFrame = m_ullCerrentTime;
		}
	}
	else
	{
		m_iImagePose = 0;
		m_eAction = NONE;
	}
}

void CBoss_CutMan::Action_Idle()
{
	if (m_ullActionTimer + 1500 > m_ullCerrentTime)
	{
		if(m_ullAnimationFrame + 300 < m_ullCerrentTime)
		{
			if (m_iImagePose == 1)
				m_iImagePose = 0;
			else
				m_iImagePose = 1;

			m_ullAnimationFrame = m_ullCerrentTime;
		}
	}
	else
	{
		m_iImagePose = 0;
		m_tInfo.fCY = 48.f;
		m_eAction = NONE;
	}
}

void CBoss_CutMan::Action_Shoot()
{

	if (m_ullActionTimer + 1000 > m_ullCerrentTime)
	{
		if(m_bShoot == false)
			m_iImagePose = 6;
		else
			m_eAction = NONE;
	}
	else if (m_ullActionTimer + 2000 > m_ullCerrentTime)
	{
		if (m_bShoot == false)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CRolling_Cutter>::Create(m_tInfo.fX, m_tInfo.fY, m_eDir, this, CObjMgr::Get_Instance()->Get_Player()));
			m_iImagePose = 7;
			m_bShoot = true;
		}
	}
	else
	{
		m_iImagePose = 0;
		m_eAction = NONE;
	}
}

void CBoss_CutMan::Action_Render(HDC hDC)
{
	TCHAR szBuf[32] = {};
	switch (m_eAction)
	{
	case NONE:
		wsprintf(szBuf, L"�Ƹ� ���� : (%s)", L"NONE");
		break;
	case RUN:
		wsprintf(szBuf, L"�Ƹ� ���� : (%s)", L"RUN");
		break;
	case IDLE:
		wsprintf(szBuf, L"�Ƹ� ���� : (%s)", L"IDLE");
		break;
	case SHOOT:
		wsprintf(szBuf, L"�Ƹ� ���� : (%s)", L"SHOOT");
		break;
	}
	TextOut(hDC, 650, 0, szBuf, lstrlen(szBuf));
}
