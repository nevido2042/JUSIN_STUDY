#include "pch.h"
#include "CFire_Storm.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"

void CFire_Storm::Initialize()
{
	m_tInfo.fY -= 40.f;
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;
	m_fSpeed = 5.f;
	m_fAngle = 5.f;
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_fire_all.bmp", L"Left_Fire");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_fire_all_X_Change.bmp", L"Right_Fire");
	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;
}

int CFire_Storm::Update()
{

	m_tInfo.fX += m_fSpeed;

	Update_Rect();

	return 0;
}

void CFire_Storm::Late_Update()
{
}

void CFire_Storm::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX(); // ȭ���� X�� ��ũ�� �ξ� ��������
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY(); // ȭ���� Y�� ��ũ�� �ξ� ��������

	// ���⿡ ���� ����� �̹��� ����
	LPCWSTR bulletImage = (m_eDir == DIR_LEFT) ? L"Left_Fire" : L"Right_Fire";
	// m_eDir ���� DIR_LEFT��� Left_Fire �̹����� ����ϰ� �ƴ϶�� "Right_Fire" �̹��� ���

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(bulletImage);

	// �̹��� ��� (���� ó�� ����)
	GdiTransparentBlt(
		hDC,                            // ��� ��� DC (ȭ�鿡 ����� HDC)
		m_tRect.left + iScrollX,        // ����� ��ġ�� X ��ǥ (���� ��ũ�� ���� ���� ���)
		m_tRect.top + iScrollY,         // ����� ��ġ�� Y ��ǥ (���� ��ũ�� ���� ���� ���)
		(int)m_tInfo.fCX + 15,          // ����� �̹����� �ʺ� (m_tInfo.fCX �Ⱥ�����..+15 �߰�)
		(int)m_tInfo.fCY + 40,          // ����� �̹����� ���� (m_tInfo.fCY �Ⱥ�����.. +40 �߰�)
 		hMemDC,                         // ����� ���� �̹����� DC (�̹��� �ڵ�)
		(m_eDir == DIR_LEFT) ? 308 : 161, // ���� �̹������� ������ ���� X ��ǥ (���⿡ ���� �ٸ�)
		(m_eDir == DIR_LEFT) ? 98 : 102,  // ���� �̹������� ������ ���� Y ��ǥ (���⿡ ���� �ٸ�)
		(int)m_tInfo.fCX + 15,          // ���� �̹������� ������ ������ �ʺ� 
		(int)m_tInfo.fCY + 40,          // ���� �̹������� ������ ������ ���� 
		RGB(128, 0, 128));             // ���� ó�� ���� (�̹������� �� ������ �����ϰ� ó��)
}

void CFire_Storm::Release()
{
}
//m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
//m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));