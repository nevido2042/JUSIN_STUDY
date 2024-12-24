#include "pch.h"
#include "Item.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"

CItem::CItem()
{
}

CItem::~CItem()
{
	Release();
}

void CItem::Initialize()
{
	m_eRenderID = RENDER_ITEM;
}

int CItem::Update()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CItem::Late_Update()
{
	Follow_Pawn();
}

void CItem::Render(HDC hDC)
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,			// ���� ���� DC
		(int)m_tRect.left + (int)fScrollX,	// ���� ���� ��ġ ��ǥ X, Y	
		(int)m_tRect.top + (int)fScrollY,
		(int)m_tInfo.fCX,			// ���� ���� �̹����� ����, ����
		(int)m_tInfo.fCY,
		hMemDC,						// ������ �̹��� DC	
		0,				// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		0,
		(int)m_tInfo.fCX,			// ������ �̹����� ����, ����
		(int)m_tInfo.fCY,
		RGB_WHITE);		// ������ ����
}

void CItem::Release()
{
}

void CItem::Follow_Pawn()
{
	if (!m_pTarget)
	{
		return;
	}

	Set_Pos(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
}
