#include "pch.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CTile::CTile() : m_eOption(OPT_END)
{
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;

	m_pImgKey = L"Ice";

	m_eOption = OPT_REACHABLE;
}

int CTile::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CTile::Late_Update()
{

}

void CTile::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	BitBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		TILECX,
		TILECY,
		hMemDC,
		TILECX * m_iDrawID,
		0,
		SRCCOPY);

	HPEN hPen(nullptr);
	HPEN hOldPen(nullptr);

	if (m_eOption == OPT_BLOCKED)
	{
		// ������ �� ����
		hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0)); // ���� 1, ������
		hOldPen = (HPEN)SelectObject(hDC, hPen);       // ���� �� ���� �� ������ �� ����
	}
	
	// �簢�� �׸���
	MoveToEx(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, nullptr);         // ���� ���� �̵�
	LineTo(hDC, m_tRect.right+ iScrollX, m_tRect.top + iScrollY);                  // ��� ��
	LineTo(hDC, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);               // ������ ��
	LineTo(hDC, m_tRect.left + iScrollX, m_tRect.bottom + iScrollY);                // �ϴ� ��
	LineTo(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY);                   // ���� ��

	if (m_eOption == OPT_BLOCKED)
	{
		// �� ����
		SelectObject(hDC, hOldPen); // ���� �� ����
		DeleteObject(hPen);         // ������ �� ����
	}

}

void CTile::Release()
{
}
