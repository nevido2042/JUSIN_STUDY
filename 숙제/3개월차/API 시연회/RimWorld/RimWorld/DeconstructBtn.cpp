#include "pch.h"
#include "DeconstructBtn.h"
#include "BmpMgr.h"

CDeconstructBtn::CDeconstructBtn()
{
}

CDeconstructBtn::~CDeconstructBtn()
{
	Release();
}

void CDeconstructBtn::On_Click()
{
	//해체 모드 변경
}

void CDeconstructBtn::Initialize()
{
	Set_ImgKey(L"Deconstruct");

	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 128.f;

	m_eRenderID = RENDER_UI;
}

void CDeconstructBtn::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0, 0, 
		(int)m_tInfo.fCX, 
		(int)m_tInfo.fCY,
		RGB_WHITE);
}
