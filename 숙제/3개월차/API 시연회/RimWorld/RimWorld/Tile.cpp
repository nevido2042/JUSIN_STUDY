#include "pch.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ColonyMgr.h"
#include "KeyMgr.h"
#include "Camera.h"

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

	m_eObjID = OBJ_TILE;
}

int CTile::Update()
{
	return CObj::Update();
}

void CTile::Late_Update()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		if (Is_MouseHovered_Scrolled()) //�� �ȵǴ°ž� â������� �ָ��ִ� ����
		{
			//������Ʈ�� �ִ�.
			if (m_pObj)
			{
				//������ ���̸� ����
				if (m_pObj->Get_ObjID() == OBJ_WALL ||
					m_pObj->Get_ObjID() == OBJ_TREE)
				{
					return;
				}
			}

			if (CColonyMgr::Get_Instance()->Get_Mode() == CColonyMgr::MODE_CONSTRUCT)//�Ǽ� ���
			{
				TASK tTask;
				tTask.eType = TASK::WALL;
				tTask.pObj = this;
				CColonyMgr::Get_Instance()->Emplace_ConstructSet(tTask);//�Ǽ� ��Ͽ� �߰�

			}
			else if (CColonyMgr::Get_Instance()->Get_Mode() == CColonyMgr::MODE_SHIP)
			{
				TASK tTask;
				tTask.pObj = this;
				tTask.eType = TASK::SHIP;
				CColonyMgr::Get_Instance()->Emplace_ConstructSet(tTask);//�Ǽ� ��Ͽ� �߰�
			}
			else if (CColonyMgr::Get_Instance()->Get_Mode() == CColonyMgr::MODE_CAMPFIRE)
			{
				TASK tTask;
				tTask.pObj = this;
				tTask.eType = TASK::CAMPFIRE;
				CColonyMgr::Get_Instance()->Emplace_ConstructSet(tTask);//�Ǽ� ��Ͽ� �߰�
			}
		}
	}
}

void CTile::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	// ���� ������ Ÿ�� ũ�� ���
	int zoomedTileCX = int(TILECX * CCamera::Get_Instance()->Get_Zoom());
	int zoomedTileCY = int(TILECY * CCamera::Get_Instance()->Get_Zoom());

	POINT tPoint = CCamera::Get_Instance()->WorldToScreen(m_tRect.left, m_tRect.top);
	// Ÿ�� ������ (StretchBlt ���)
	StretchBlt(
		hDC,
		tPoint.x,
		tPoint.y,
		zoomedTileCX,
		zoomedTileCY,
		hMemDC,
		0,
		0,
		TILECX,
		TILECY,
		SRCCOPY // �ܼ� ����
	);

	//BitBlt(
	//	hDC,
	//	tPoint.x,        // Ÿ���� ȭ�� ��ǥ X
	//	tPoint.y,        // Ÿ���� ȭ�� ��ǥ Y
	//	TILECX,          // ���� Ÿ���� �ʺ� (ũ�� ����)
	//	TILECY,          // ���� Ÿ���� ���� (ũ�� ����)
	//	hMemDC,
	//	0,
	//	0,
	//	SRCCOPY          // �ܼ� ����
	//);

	//HPEN hPen(nullptr);
	//HPEN hOldPen(nullptr);

	//if (m_eOption == OPT_BLOCKED)
	//{
	//	// ������ �� ����
	//	hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0)); // ���� 1, ������
	//	hOldPen = (HPEN)SelectObject(hDC, hPen);       // ���� �� ���� �� ������ �� ����
	//}
	//
	//// �簢�� �׸���
	////MoveToEx(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, nullptr);         // ���� ���� �̵�
	////LineTo(hDC, m_tRect.right+ iScrollX, m_tRect.top + iScrollY);                  // ��� ��
	////LineTo(hDC, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);               // ������ ��
	////LineTo(hDC, m_tRect.left + iScrollX, m_tRect.bottom + iScrollY);                // �ϴ� ��
	////LineTo(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY);                   // ���� ��

	//// �簢�� �׸���
	//if (m_eOption == OPT_BLOCKED)
	//{
	//	// ���� ��ǥ�� ȭ�� ��ǥ�� ��ȯ
	//	POINT topLeft = CCamera::Get_Instance()->WorldToScreen(m_tRect.left, m_tRect.top);
	//	POINT bottomRight = CCamera::Get_Instance()->WorldToScreen(m_tRect.right, m_tRect.bottom);

	//	// �� ����
	//	int zoomedLeft = topLeft.x;
	//	int zoomedTop = topLeft.y;
	//	int zoomedRight = bottomRight.x;
	//	int zoomedBottom = bottomRight.y;

	//	// �簢�� �׸���
	//	MoveToEx(hDC, zoomedLeft, zoomedTop, nullptr);             // ���� ���� �̵�
	//	LineTo(hDC, zoomedRight, zoomedTop);                       // ��� ��
	//	LineTo(hDC, zoomedRight, zoomedBottom);                    // ������ ��
	//	LineTo(hDC, zoomedLeft, zoomedBottom);                     // �ϴ� ��
	//	LineTo(hDC, zoomedLeft, zoomedTop);                        // ���� ��
	//}


	//if (m_eOption == OPT_BLOCKED)
	//{
	//	// �� ����
	//	SelectObject(hDC, hOldPen); // ���� �� ����
	//	DeleteObject(hPen);         // ������ �� ����
	//}

}

void CTile::Release()
{
}
