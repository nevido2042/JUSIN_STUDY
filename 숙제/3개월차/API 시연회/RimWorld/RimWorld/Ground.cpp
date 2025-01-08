#include "pch.h"
#include "Ground.h"
#include "BmpMgr.h"
#include "Camera.h"

CGround::CGround()
	:m_pImage(nullptr)
{
}

CGround::~CGround()
{
	Release();
}

void CGround::Initialize()
{
	// GDI+�� PNG �̹��� �ε�
	//m_pImage = Gdiplus::Image::FromFile(L"../Image/Terrain/Gravel.png");

	Set_Size(TILECX * TILEX, TILECY * TILEY);

	m_eRenderID = RENDER_BACKGROUND;
}

void CGround::Late_Update()
{
}

void CGround::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Gravel");

	// ���� ������ Ÿ�� ũ�� ���
	float fZoom = CCamera::Get_Instance()->Get_Zoom();
	POINT tPoint = CCamera::Get_Instance()->WorldToScreen(m_tRect.left, m_tRect.top);
	// Ÿ�� ������ (StretchBlt ���)
	StretchBlt(
		hDC,
		tPoint.x,
		tPoint.y,
		(int)(m_tInfo.fCX * fZoom),
		(int)(m_tInfo.fCY * fZoom),
		hMemDC,
		0,
		0,
		m_tInfo.fCX,
		m_tInfo.fCY,
		SRCCOPY // �ܼ� ����
	);

	//// GDI+ Graphics ��ü ����
	//Gdiplus::Graphics graphics(hDC);

	//// �ܰ� ȭ�� ��ǥ ���
	//float fZoom = CCamera::Get_Instance()->Get_Zoom();
	//POINT tPoint = CCamera::Get_Instance()->WorldToScreen(m_tRect.left, m_tRect.top);

	//// ��� ���� ��� (�� �ݿ�)
	//Gdiplus::RectF destRect(
	//	(float)tPoint.x,
	//	(float)tPoint.y,
	//	m_tInfo.fCX * fZoom,
	//	m_tInfo.fCY * fZoom
	//);

	//// PNG �̹��� ������
	//graphics.DrawImage(m_pImage, destRect);
}

void CGround::Release()
{
}
