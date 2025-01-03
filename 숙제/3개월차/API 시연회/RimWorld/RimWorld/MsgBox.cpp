#include "pch.h"
#include "MsgBox.h"
#include "BmpMgr.h"

CMsgBox::CMsgBox()
{
}

CMsgBox::~CMsgBox()
{
	Release();
}

void CMsgBox::Initialize()
{
	Set_ImgKey(L"MsgBox");
	Set_Size(256, 256);
	m_eRenderID = RENDER_UI;

	//우주선에 탑승한 림의 인원 수 파악
}

int CMsgBox::Update()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CMsgBox::Late_Update()
{
}

void CMsgBox::Render(HDC hDC)
{
	/*if (!m_bActivate)
	{
		return;
	}*/

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	BitBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0,
		0,
		SRCCOPY);
}

void CMsgBox::Release()
{
}
