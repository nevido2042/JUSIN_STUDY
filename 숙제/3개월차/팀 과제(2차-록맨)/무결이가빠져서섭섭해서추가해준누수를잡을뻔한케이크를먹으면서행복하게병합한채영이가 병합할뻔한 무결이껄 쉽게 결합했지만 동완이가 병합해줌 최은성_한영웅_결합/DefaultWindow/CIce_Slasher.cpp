#include "pch.h"
#include "CIce_Slasher.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"




void CIce_Slasher::Initialize()
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_fSpeed = 5.f;

	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_ice_all.bmp", L"BossIceAll");
}

void CIce_Slasher::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BossIceAll");

	GdiTransparentBlt(hDC,            // ���� ���� DC
		m_tRect.left + iScrollX,    // ���� ���� ��ġ ��ǥ X, Y    
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,            // ���� ���� �̹����� ����, ����    ����Ʈ - ����Ʈ �ؼ� ���̸� �־�����ϴϱ�
		(int)m_tInfo.fCY,
		hMemDC,                        // ������ �̹��� DC    
		553,                            // ��Ʈ�� ��� ���� ��ǥ(Left, top)
		21,
		(int)m_tInfo.fCX,            // ������ �̹����� ����, ����
		(int)m_tInfo.fCY,
		RGB(128, 0, 128));            // ������ ����
}

void CIce_Slasher::OnCollision(CObj* _pOther, OBJID _eOtherID)
{
	if (_eOtherID == OBJ_PLAYER)
	{
		//_pOther->Set_
	}
}

int CIce_Slasher::Update()
{
	m_tInfo.fX += m_fSpeed;

	__super::Update_Rect();

	return 0;
}

void CIce_Slasher::Late_Update()
{
}

void CIce_Slasher::Release()
{
}
