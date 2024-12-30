#include "pch.h"
#include "RimSlot.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "ColonyMgr.h"
#include "Rim.h"

CRimSlot::CRimSlot()
{
}

CRimSlot::~CRimSlot()
{
	Release();
}

void CRimSlot::Initialize()
{
	Set_Size(64.f, 64.f);

	m_eRenderID = RENDER_UI;
}

int CRimSlot::Update()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CRimSlot::Late_Update()
{
	if (Is_MouseHovered())
	{
		if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
		{
			CScrollMgr::Get_Instance()->Set_ScrollX(-Get_Target()->Get_Info().fX + WINCX * 0.5f);
			CScrollMgr::Get_Instance()->Set_ScrollY(-Get_Target()->Get_Info().fY + WINCY * 0.5f);

			CColonyMgr::Get_Instance()->Set_Target(m_pTarget);
		}
	}
}

void CRimSlot::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"DesButBG");

	BitBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0, 0,
		SRCCOPY);

	if (CRim* pPawn = dynamic_cast<CRim*>(m_pTarget))
	{
		//캐릭터 그리기
		HDC hMemDC(nullptr);
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(pPawn->Get_ImgKeyArr_Body()->c_str());
		GdiTransparentBlt(hDC,
			m_tRect.left,
			m_tRect.top,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			0, 0,
			128, 128,
			RGB_PURPLE);
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(pPawn->Get_ImgKeyArr_Face()->c_str());
		GdiTransparentBlt(hDC,
			m_tRect.left,
			m_tRect.top - HEAD_OFFSET/2,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			0, 0,
			128, 128,
			RGB_PURPLE);
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(pPawn->Get_ImgKeyArr_Hair()->c_str());
		GdiTransparentBlt(hDC,
			m_tRect.left,
			m_tRect.top - HEAD_OFFSET/2,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			0, 0,
			128, 128,
			RGB_PURPLE);

		//죽었으면 X자 표시
		if (pPawn->Get_IsDead())
		{
			HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"DeadColonist");
			GdiTransparentBlt(hDC,
				m_tRect.left,
				m_tRect.top,
				(int)m_tInfo.fCX,
				(int)m_tInfo.fCY,
				hMemDC,
				0, 0,
				48, 48,
				RGB_WHITE);
		}
	}
}

void CRimSlot::Release()
{
}
