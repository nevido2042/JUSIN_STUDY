#include "pch.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ColonyMgr.h"
#include "KeyMgr.h"

CTile::CTile() : m_eOption(OPT_END), m_pObj(nullptr)
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
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CTile::Late_Update()
{
	if (Is_MouseHovered_Scrolled() && 
		CColonyMgr::Get_Instance()->Get_Mode() == CColonyMgr::MODE_CONSTRUCT&&
		!m_pObj)//Ÿ������ ���� ������
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
		{
			TASK tTask;
			tTask.pObj = this;
			CColonyMgr::Get_Instance()->Emplace_ConstructSet(tTask);//�Ǽ� ��Ͽ� �߰�
		}
	}
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
