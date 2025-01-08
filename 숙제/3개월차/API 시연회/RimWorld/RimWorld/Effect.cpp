#include "pch.h"
#include "Effect.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"

CEffect::CEffect()
	:m_fLifeTime(0.f), m_fTime(0.f)
{
}

CEffect::~CEffect()
{
	Release();
}

void CEffect::Initialize()
{
	m_eRenderID = RENDER_EFFECT;
}

int CEffect::Update()
{
	return CObj::Update();
}

void CEffect::Late_Update()
{
	m_fTime += GAMESPEED;

	if (m_fTime > m_fLifeTime)
	{
		Set_Destroyed();
	}
}

void CEffect::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	float	fZoom = CCamera::Get_Instance()->Get_Zoom();

	POINT tPoint = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left, (float)m_tRect.top);

	GdiTransparentBlt(hDC,			// ���� ���� DC
		tPoint.x,//m_tRect.left + iScrollX // ���� ���� ��ġ ��ǥ X, Y	
		tPoint.y,//m_tRect.top + iScrollY,
		(int)(m_tInfo.fCX * fZoom),			// ���� ���� �̹����� ����, ����
		(int)(m_tInfo.fCY * fZoom),
		hMemDC,						// ������ �̹��� DC	
		0,// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		0,
		(int)m_tInfo.fCX,			// ������ �̹����� ����, ����
		(int)m_tInfo.fCY,
		RGB_WHITE);		// ������ ����
}

void CEffect::Release()
{
}
