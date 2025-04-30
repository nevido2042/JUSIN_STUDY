#include "pch.h"
#include "CBullet_Normal.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CBullet_Normal::CBullet_Normal()
{
}

CBullet_Normal::~CBullet_Normal()
{
	Release();
}

void CBullet_Normal::Initialize()
{
	m_tInfo.fCX = 17.f;
	m_tInfo.fCY = 13.f;

	m_fSpeed = 5.f;

	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/player_weapon_all.bmp", L"Player_Bullet");
}

int CBullet_Normal::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += m_fSpeed;

	__super::Update_Rect();

	return 0;
}

void CBullet_Normal::Late_Update()
{
}

void CBullet_Normal::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Bullet");
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
		m_tInfo.fCX,                         // ������� �̹����� ���� ���� (����� �̹��� ũ�� ����)
		m_tInfo.fCY,                         // ������� �̹����� ���� ����
		hMemDC,                     // ������ �̹����� ����� �޸� DC
		507,                        // ���� ��Ʈ�ʿ��� ���縦 ������ X��ǥ
		25,                        // ���� ��Ʈ�ʿ��� ���縦 ������ Y��ǥ
		m_tInfo.fCX,                         // ���� ��Ʈ�ʿ��� ������ ������ ���� ����
		m_tInfo.fCY,                         // ���� ��Ʈ�ʿ��� ������ ������ ���� ����
		RGB(128, 0, 128));       // ���� ó���� ���� (�̹����� �� ������ ���� ó����)
}

void CBullet_Normal::Release()
{
}
