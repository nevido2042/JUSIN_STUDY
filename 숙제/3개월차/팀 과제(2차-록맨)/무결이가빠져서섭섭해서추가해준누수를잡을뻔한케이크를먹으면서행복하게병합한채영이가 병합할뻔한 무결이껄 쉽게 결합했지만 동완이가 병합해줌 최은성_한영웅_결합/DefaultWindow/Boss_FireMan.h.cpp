#include "pch.h"
#include "Boss_FireMan.h"
#include "CBullet.h"
#include "CScrollMgr.h"
#include "CLineMgr.h"
#include "BlockMgr.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CFire_Storm.h"
#include "CObjMgr.h"

CBoss_FireMan::CBoss_FireMan()
{
	ZeroMemory(&m_p_Boss_Bullet, sizeof(m_p_Boss_Bullet));
	ZeroMemory(&HP_INFO, sizeof(HP_INFO));
	m_Boss_pPlayer = NULL;
	m_ullLast_Fire = 0;

	Hp_Count = 0;

	m_bJump = false;
	m_fJumpPower=  0.f;
	m_fTime = 0.f;
	m_ullLast_Fire = 0;
	m_ullLast_Fire = GetTickCount64();
	////////////////////////
	bool m_bMovingLeft;
	int m_fJumpCooldown;

	MAX_Hp = 0;
	Hp_Count = 0;// ������ �浹�Ҷ����� üũ
	m_Boss_Ground = true;

	m_pPlayer = nullptr;
	bool m_Boss_Ground;

}

CBoss_FireMan::~CBoss_FireMan()
{
}

void CBoss_FireMan::Initialize()
{

	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_fire_all.bmp",L"Fire_Man");

	m_tInfo = { 300, 400, 42.f, 48.f };
	HP_INFO = { 200,200,200.f,200.f };
	m_fSpeed = 3.f;
	MAX_Hp = 10;
	 
}
//CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CFire_Storm>::Create(m_tInfo.fX, m_tInfo.fY, DIR_RIGHT));
int CBoss_FireMan::Update()
{
	m_tInfo.fY += 1.f;

	m_tInfo.fX += m_fSpeed;

	if (m_tInfo.fX > 700|| m_tInfo.fX<150)
	{
		m_fSpeed *= -1;
	}

	if (m_ullLast_Fire + 800 < GetTickCount64()) // 0.8�ʿ� �ѹ��� �߻�
	{
		m_ullLast_Fire = GetTickCount64(); // ������ �߻� �ð�����

		if (m_tInfo.fX > m_pPlayer->Get_Info().fX) // �÷��̾��� ���ʿ� ���� ��
		{
			CObjMgr::Get_Instance()->Add_Object

			(OBJ_BOSSBULLET,CAbstractFactory<CFire_Storm>::Create(m_tInfo.fX, m_tInfo.fY, DIR_LEFT)

			);
		}
		else // �÷��̾��� �����ʿ� ���� ��
		{
			CObjMgr::Get_Instance()->Add_Object(
				OBJ_BOSSBULLET,
				CAbstractFactory<CFire_Storm>::Create(m_tInfo.fX, m_tInfo.fY, DIR_RIGHT)
			);
		}
	}

	//Jumping();

	// ���� ����
	if (!m_bJump && rand() % 100 < 20) // 20% Ȯ���� ���� ����
	{
		m_bJump = true;
		m_fJumpPower = 15.f; // ���� �Ŀ� ����
		m_fTime = 0.f;       // ���� �ð� �ʱ�ȭ
	}

	Update_Rect();
	return OBJ_NOEVENT;
}


void CBoss_FireMan::Late_Update()
{
}


void CBoss_FireMan::Render(HDC hDC)
{
	// ��ũ�� ��ġ ���
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMeDC = CBmpMgr::Get_Instance()->Find_Image(L"Fire_Man");

	// ���� �̹��� ������
	GdiTransparentBlt(
    hDC,
    m_tRect.left + iScrollX,              // X ��ǥ
    m_tRect.top + iScrollY - 29,          // Y ��ǥ�� ���� (������ �ٴڿ� �°� �������ǵ��� ������ �߰�)
    68,                                   // �̹��� ���� ũ��
    77,                                   // �̹��� ���� ũ��
    hMeDC,
    51,
    15,
    68,
    77,
    RGB(128, 0, 128));



	// ü�� �� �׸���
	Rectangle(hDC, 90, 80, 110, 200); // ��ü ü�� �� (�ִ� ü��)

	// ü�� �� ���� ����
	HBRUSH MyBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);

	// ���� ü�¿� ���� ü�� �� ���� ���&
	Rectangle(hDC, 90, 80, 110, (200 - Hp_Count));

	// �귯�� ����
	SelectObject(hDC, OldBrush);
	DeleteObject(MyBrush);
}

CObj* CBoss_FireMan::Create_Bullet(int _type)
{
	return nullptr;
}








