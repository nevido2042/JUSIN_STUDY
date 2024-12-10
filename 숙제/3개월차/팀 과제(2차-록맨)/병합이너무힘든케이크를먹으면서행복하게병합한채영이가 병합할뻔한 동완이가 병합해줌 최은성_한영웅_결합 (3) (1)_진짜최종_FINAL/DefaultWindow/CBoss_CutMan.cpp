#include "pch.h"
#include "CBoss_CutMan.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CRolling_Cutter.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

#include "CUIMgr.h"
#include "CBossHpBar.h"
#include "CEffect.h"


CBoss_CutMan::CBoss_CutMan() : m_iImagePose(0), m_iAnimationX(0),
m_iAnimationY(0), m_eAction(CUT_NONE), m_bShoot(false), m_bRunAni(true), m_iYInter()
{
}

CBoss_CutMan::~CBoss_CutMan()
{
}

void CBoss_CutMan::Initialize()
{
	m_iHp = 80;
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;

	m_tAnimationSize = { 0,0 };
	m_fJumpPower = 0.f;

	m_fSpeed = 3.f;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_cut_all.bmp", L"CutMan");
	m_eBossType = BT_ICEMAN;
	CUIMgr::Get_Instance()->Add_UI(UI_HP_BOSS, CAbstractFactory<CBossHpBar>::CreateUI(this));
}

int CBoss_CutMan::Update()
{

	if (m_iHp <= 0)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CEffect>::Create(m_tInfo.fX - 10.f, m_tInfo.fY - 5.f));
		return OBJ_DEAD;
	}

	Jumping();

	m_ullCerrentTime = GetTickCount64();
	if (m_eAction == CUT_NONE && !m_bJump)
	{
		Select_Action();
	}
	else
	{
		switch (m_eAction)
		{
		case CUT_RUN:
			Action_Run();
			break;
		case CUT_SHOOT:
			Action_Shoot();
			break;
		case CUT_IDLE:
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
}

void CBoss_CutMan::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CutMan");

	GdiTransparentBlt(hDC,            // ���� ���� DC
		m_tRect.left + iScrollX - m_tAnimationSize.x,    // ���� ���� ��ġ ��ǥ X, Y    
		m_tRect.top + iScrollY - m_tAnimationSize.y + m_iYInter,
		(int)m_tInfo.fCX + m_tAnimationSize.x,            // ���� ���� �̹����� ����, ����
		(int)m_tInfo.fCY + m_tAnimationSize.y - m_iYInter,
		hMemDC,                        // ������ �̹��� DC    
		m_iAnimationX - m_tAnimationSize.x,                            // ��Ʈ�� ��� ���� ��ǥ(Left, top)
		m_iAnimationY - m_tAnimationSize.y,
		(int)m_tInfo.fCX + m_tAnimationSize.x,            // ������ �̹����� ����, ����
		(int)m_tInfo.fCY + m_tAnimationSize.y - m_iYInter,
		RGB(128, 0, 128));

	Action_Render(hDC);
}

void CBoss_CutMan::Release()
{
}
void CBoss_CutMan::Update_Animation()
{

	if (m_bShoot)
		m_tAnimationSize.y = 0;
	else
		m_tAnimationSize.y = 17;

	if (m_iImagePose == 7)
		m_tAnimationSize.x = 14;
	else
		m_tAnimationSize.x = 0;

	switch (m_iImagePose)
	{
	case 0:
		m_iAnimationX = 10;
		m_iAnimationY = 26;
		m_iYInter = 0;
		break;
	case 1:
		m_iAnimationX = 80;
		m_iAnimationY = 26;
		m_iYInter = 0;
		break;
	case 3:
		m_iAnimationX = 230;
		m_iAnimationY = 32;
		m_iYInter = 6;
		break;
	case 4:
		m_iAnimationX = 297;
		m_iAnimationY = 28;
		m_iYInter = 2;
		break;
	case 5:
		m_iAnimationX = 355;
		m_iAnimationY = 32;
		m_iYInter = 6;
		break;
	case 6:
		m_iAnimationX = 413;
		m_iAnimationY = 26;
		m_iYInter = 0;
		break;
	case 7:
		m_iAnimationX = 497;
		m_iAnimationY = 26;
		m_iYInter = 0;

		break;
	}
}

void CBoss_CutMan::Select_Action()
{
	m_eAction = (CUTMAN_ACTION)(rand() % 3 + 1);
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
		m_eAction = CUT_NONE;
	}
}

void CBoss_CutMan::Action_Idle()
{
	if (m_ullActionTimer + 1500 > m_ullCerrentTime)
	{
		if (m_ullAnimationFrame + 300 < m_ullCerrentTime)
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
		m_eAction = CUT_NONE;
	}
}

void CBoss_CutMan::Action_Shoot()
{

	if (m_ullActionTimer + 1000 > m_ullCerrentTime)
	{
		if (m_bShoot == false)
			m_iImagePose = 6;
		else
			m_eAction = CUT_NONE;
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
		m_eAction = CUT_NONE;
	}
}

void CBoss_CutMan::Action_Render(HDC hDC)
{
	TCHAR szBuf[32] = {};
	switch (m_eAction)
	{
	case CUT_NONE:
		wsprintf(szBuf, L"�Ƹ� ���� : (%s)", L"NONE");
		break;
	case CUT_RUN:
		wsprintf(szBuf, L"�Ƹ� ���� : (%s)", L"RUN");
		break;
	case CUT_IDLE:
		wsprintf(szBuf, L"�Ƹ� ���� : (%s)", L"IDLE");
		break;
	case CUT_SHOOT:
		wsprintf(szBuf, L"�Ƹ� ���� : (%s)", L"SHOOT");
		break;
	}
	TextOut(hDC, 650, 0, szBuf, lstrlen(szBuf));

	wsprintf(szBuf, L"�Ƹ� ��ġ : (%d,%d)", (int)m_tInfo.fX, (int)m_tInfo.fY);
	TextOut(hDC, 650, 50, szBuf, lstrlen(szBuf));


}
