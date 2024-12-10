#include "pch.h"
#include "CBullet.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CAnimMgr.h"
#include "CAbstractFactory.h"
CBullet::CBullet()
{
}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/player_weapon_all.bmp", L"Bullet");

	/*CAnimMgr::Get_Instance()->Insert_Animation(L"Bullet",
		CAbstractFactory<CAnimation>::Create(this,
			FPOINT(32.f, 30.f),
			80,
			3,
			L"Bullet"));*/


	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_fSpeed = 5.f;

	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;
}

int CBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));

	__super::Update_Rect();

	return OBJ_NOEVENT;
}
void CBullet::Late_Update()
{

}

void CBullet::Render(HDC hDC)
{


	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Bullet");
	/*CAnimMgr::Get_Instance()->Render(hDC, L"Bullet");*/
	//BitBlt(hDC,						// ���� ���� DC
	//	m_tRect.left + iScrollX,	// ���� ���� ��ġ ��ǥ X, Y	
	//	m_tRect.top + iScrollY,// ���� ���� ��ġ ��ǥ X, Y	
	//	(int)m_tInfo.fCX,		// ���� ���� �̹����� ����, ����
	//	(int)m_tInfo.fCY,
	//	hMemDC,						// ������ �̹��� DC
	//	0,//����Ʈ							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
	//	0,
	//	SRCCOPY);					// ��� ȿ�� ����(�״�� ���)

	GdiTransparentBlt(
		hDC,                        // ���� ���� DC (ȭ�鿡 ����� DC)
		m_tRect.left + iScrollX,    // ������� ��ġ�� X��ǥ (���� ��ũ�� ���� ����Ͽ� ���)
		m_tRect.top + iScrollY,     // ������� ��ġ�� Y��ǥ (���� ��ũ�� ���� ����Ͽ� ���)
		32,                         // ������� �̹����� ���� ���� (����� �̹��� ũ�� ����)
		31,                         // ������� �̹����� ���� ����
		hMemDC,                     // ������ �̹����� ����� �޸� DC
		473,                        // ���� ��Ʈ�ʿ��� ���縦 ������ X��ǥ
		616,                        // ���� ��Ʈ�ʿ��� ���縦 ������ Y��ǥ
		32,                         // ���� ��Ʈ�ʿ��� ������ ������ ���� ����
		31,                         // ���� ��Ʈ�ʿ��� ������ ������ ���� ����
		RGB(128, 0, 128));       // ���� ó���� ���� (�̹����� �� ������ ���� ó����)
}


void CBullet::Release()
{
}

void CBullet::Default_Bullet()
{
	m_tInfo.fCX = 20.f; // �Ѿ� ũ��
	m_tInfo.fCY = 20.f;
	m_fSpeed = 10.f;

	Check_Bul = Default;
}

void CBullet::Skill_F1_Bullet()
{

	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;

	m_fSpeed = 15.f;

	Check_Bul = Skill_1;

}

void CBullet::Skill_F2_Bullet()
{

	m_tInfo.fCX = 20.f; // ũ��
	m_tInfo.fCY = 20.f;
	m_tRect.right = 50;
	m_tRect.left = 50;


	m_fSpeed = 20.f; //�Ѿ˼ӵ�

	Check_Bul = Skill_2;

}

