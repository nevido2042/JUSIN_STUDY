#include "pch.h"
#include "SteelWallBtn.h"
#include "BmpMgr.h"
#include "ColonyMgr.h"

CSteelWallBtn::CSteelWallBtn()
{
}

CSteelWallBtn::~CSteelWallBtn()
{
	Release();
}

void CSteelWallBtn::On_Click()
{
	//건축 모드 변경
	CColonyMgr::Get_Instance()->Change_Mode(CColonyMgr::MODE_CONSTRUCT);
}

void CSteelWallBtn::Initialize()
{
	Set_ImgKey(L"RockSmooth_MenuIcon");

	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 64.f;

	m_eRenderID = RENDER_UI;
}

void CSteelWallBtn::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	BitBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC, 0, 0, SRCCOPY);
}
