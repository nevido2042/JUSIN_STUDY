#include "pch.h"
#include "Campfire.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CCampfire::CCampfire()
{
}

CCampfire::~CCampfire()
{
	Release();
}

void CCampfire::Initialize()
{
	Set_Size(64.f, 64.f);

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	//m_tFrame.ullSpeed = 200;
	m_tFrame.ullTime = 10;

	m_eRenderID = RENDER_GAMEOBJECT;
}

void CCampfire::Late_Update()
{
	__super::Move_Frame();
}

void CCampfire::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hFireDC = CBmpMgr::Get_Instance()->Find_Image(L"FireAnimated");
	HDC hCampfireDC = CBmpMgr::Get_Instance()->Find_Image(L"Campfire");

	GdiTransparentBlt(hDC,			// ���� ���� DC
		m_tRect.left + iScrollX,	// ���� ���� ��ġ ��ǥ X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// ���� ���� �̹����� ����, ����
		(int)m_tInfo.fCY,
		hCampfireDC,						// ������ �̹��� DC	
		0,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		0,
		(int)m_tInfo.fCX,			// ������ �̹����� ����, ����
		(int)m_tInfo.fCY,
		RGB_WHITE);		// ������ ����

	GdiTransparentBlt(hDC,			// ���� ���� DC
		m_tRect.left + iScrollX + 5,	// ���� ���� ��ġ ��ǥ X, Y	
		m_tRect.top + iScrollY - 5,
		(int)(m_tInfo.fCX * 0.9f),			// ���� ���� �̹����� ����, ����
		(int)(m_tInfo.fCY * 0.9f),
		hFireDC,						// ������ �̹��� DC	
		(int)m_tInfo.fCX * m_tFrame.iFrameStart,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		(int)m_tInfo.fCY * m_tFrame.iMotion,
		(int)m_tInfo.fCX,			// ������ �̹����� ����, ����
		(int)m_tInfo.fCY,
		RGB_PURPLE);		// ������ ����
}

void CCampfire::Release()
{
}
