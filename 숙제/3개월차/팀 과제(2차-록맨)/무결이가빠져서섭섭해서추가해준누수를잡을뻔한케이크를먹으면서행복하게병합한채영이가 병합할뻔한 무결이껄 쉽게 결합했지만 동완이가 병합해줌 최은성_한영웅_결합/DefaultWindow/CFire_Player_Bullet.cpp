#include "pch.h"
#include "CBullet.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CAnimMgr.h"
#include "CAbstractFactory.h"
#include "CFire_Player_Bullet.h"
#include "CObjMgr.h"

CFire_Player_Bullet::CFire_Player_Bullet()
{
}

CFire_Player_Bullet::~CFire_Player_Bullet()
{
}


void CFire_Player_Bullet::Initialize()
{
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;
	m_fSpeed = 5.f;
	m_tInfo.fX = m_pPlayer->Get_Info().fX;
	m_tInfo.fY = m_pPlayer->Get_Info().fY;

	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/player_weapon_all.bmp", L"Player_Bullet");


}

int CFire_Player_Bullet::Update()
{
	
	m_tInfo.fX += m_fSpeed;
	


	__super::Update_Rect();

	return 0;
}
void CFire_Player_Bullet::Late_Update()
{

}

void CFire_Player_Bullet::Render(HDC hDC)
{

	
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Bullet");
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




void CFire_Player_Bullet::Release()
{
}

