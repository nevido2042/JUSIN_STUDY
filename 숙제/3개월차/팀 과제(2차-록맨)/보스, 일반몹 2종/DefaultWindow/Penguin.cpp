#include "pch.h"
#include "Penguin.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

void CPenguin::Initialize()
{
    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 50.f;
    m_fSpeed = 3.f;

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../megaman1 sprite/enemy_all.bmp", L"EnemyAll");
}

void CPenguin::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"EnemyAll");

	GdiTransparentBlt(hDC,            // ���� ���� DC
		m_tRect.left + iScrollX,    // ���� ���� ��ġ ��ǥ X, Y    
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,            // ���� ���� �̹����� ����, ����    ����Ʈ - ����Ʈ �ؼ� ���̸� �־�����ϴϱ�
		(int)m_tInfo.fCY,
		hMemDC,                        // ������ �̹��� DC    
		215,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		349,                           
		(int)m_tInfo.fCX,            // ������ �̹����� ����, ����
		(int)m_tInfo.fCY,
		RGB(128, 0, 128));
}

int CPenguin::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	Set_PosX(-5.f);

	Set_Pos(Get_Info().fX, 50.f * sinf(Get_Info().fX * PI / 180.f) + 400.f);

	if (Get_Info().fX < -iScrollX)
	{
		Set_Pos(WINCX - iScrollX, Get_Info().fY);
	}

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CPenguin::Late_Update()
{
}

void CPenguin::Release()
{
}
