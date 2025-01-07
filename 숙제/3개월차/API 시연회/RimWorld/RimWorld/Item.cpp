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
	return CObj::Update();
}

void CItem::Late_Update()
{
	Follow_Pawn();
}

void CItem::Render(HDC hDC)
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
	float fZoom = CCamera::Get_Instance()->Get_Zoom(); // �� ���� ��������

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	// ���� ��ǥ�� ��ũ�� ��ǥ�� ��ȯ (�� �ݿ�)
	POINT tScreenPoint = CCamera::Get_Instance()->WorldToScreen(m_tInfo.fX, m_tInfo.fY);

	// �̹��� ũ��� ��ǥ�� �� ���� �ݿ�
	GdiTransparentBlt(hDC,                           // ���� ���� DC
		(int)(tScreenPoint.x - (m_tInfo.fCX / 2) * fZoom), // �߽��� ���� ��ǥ ���� (����)
		(int)(tScreenPoint.y - (m_tInfo.fCY / 2) * fZoom), // �߽��� ���� ��ǥ ���� (����)
		(int)(m_tInfo.fCX * fZoom),                  // ���� ���� �̹����� ���� (�� �ݿ�)
		(int)(m_tInfo.fCY * fZoom),                  // ���� ���� �̹����� ���� (�� �ݿ�)
		hMemDC,                                      // ������ �̹��� DC
		0,                                           // ��Ʈ�� ��� ���� ��ǥ X (����)
		0,                                           // ��Ʈ�� ��� ���� ��ǥ Y (����)
		(int)m_tInfo.fCX,                            // ������ �̹����� ���� ���� ũ��
		(int)m_tInfo.fCY,                            // ������ �̹����� ���� ���� ũ��
		RGB_WHITE);                                  // ������ ����

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
