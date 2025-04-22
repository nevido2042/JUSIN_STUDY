#include "pch.h"
#include "Penguin.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CPenguin::CPenguin()
{
	ZeroMemory(&m_tFly1, sizeof(IMAGE));
	ZeroMemory(&m_tFly2, sizeof(IMAGE));
	ZeroMemory(&m_tCurImage, sizeof(IMAGE));
}

CPenguin::~CPenguin()
{
	Release();
}

void CPenguin::Initialize()
{
	m_iHp = 10;
		
    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 50.f;
    m_fSpeed = 3.f;

	m_tFly1 = { 215,349 };
	m_tFly2 = { 268,343 };

	m_tCurImage = m_tFly1;

	m_fTime = (float)GetTickCount64();

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"EnemyAll");
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
		m_tCurImage._iX,//215, //269, 343						// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		m_tCurImage._iY,//349,                           
		(int)m_tInfo.fCX,            // ������ �̹����� ����, ����
		(int)m_tInfo.fCY,
		RGB(128, 0, 128));
}

int CPenguin::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

	if (m_fTime + 1000.f < (float)GetTickCount64())
	{
		if (m_tCurImage == m_tFly1)
		{
			m_tCurImage = m_tFly2;
		}
		else
		{
			m_tCurImage = m_tFly1;
		}
	}

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
