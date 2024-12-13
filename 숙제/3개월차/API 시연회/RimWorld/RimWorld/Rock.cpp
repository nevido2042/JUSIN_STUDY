#include "pch.h"
#include "Rock.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"

CRock::CRock()
	:m_eCurState(END), m_ePreState(END), m_iRenderX(0), m_iRenderY(0), m_bCheckNeighbor(false)
{
}

CRock::~CRock()
{
    Release();
}

void CRock::Change_Image()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CRock::SOLO:
			m_iRenderX = 0;
			m_iRenderY = 3;
			break;
		case CRock::HORIZONTAL:
			m_iRenderX = 2;
			m_iRenderY = 1;
			break;
		case CRock::VERTICAL:
			m_iRenderX = 1;
			m_iRenderY = 2;
			break;
		case CRock::END_LEFT:
			m_iRenderX = 2;
			m_iRenderY = 3;
			break;
		case CRock::END_RIGHT:
			m_iRenderX = 0;
			m_iRenderY = 1;
			break;
		case CRock::END_TOP:
			m_iRenderX = 0;
			m_iRenderY = 2;
			break;
		case CRock::END_BOTTOM:
			m_iRenderX = 1;
			m_iRenderY = 3;
			break;

		case CRock::RIGHT_BOTTOM:
			m_iRenderX = 1;
			m_iRenderY = 1;
			break;
		case CRock::LEFT_BOTTOM:
			m_iRenderX = 3;
			m_iRenderY = 3;
			break;
		case CRock::RIGHT_TOP:
			m_iRenderX = 0;
			m_iRenderY = 0;
			break;
		case CRock::LEFT_TOP:
			m_iRenderX = 2;
			m_iRenderY = 2;
			break;

		case CRock::T12:
			m_iRenderX = 2;
			m_iRenderY = 0;
			break;

		case CRock::T3:
			m_iRenderX = 1;
			m_iRenderY = 0;
			break;

		case CRock::T6:
			m_iRenderX = 3;
			m_iRenderY = 1;
			break;

		case CRock::T9:
			m_iRenderX = 3;
			m_iRenderY = 2;
			break;

		case CRock::CROSS:
			m_iRenderX = 3;
			m_iRenderY = 0;
			break;
		}

		m_ePreState = m_eCurState;
	}

}

void CRock::Initialize()
{
	Set_ImgKey(L"Rock_Atlas");
	//m_pImgKey = L"Rock_Atlas";

	m_tInfo.fCX = 80.f; //���߿� ������ 64(�浹����)+16(����) �̷������� ����ҵ�
    m_tInfo.fCY = 80.f;

	m_eCurState = SOLO;
	m_ePreState = SOLO;

	m_iRenderX = 0;
	m_iRenderY = 3;

	m_eRenderID = RENDER_GAMEOBJECT;

	m_bCheckNeighbor = true;

	//���� ���� �� �ϴ� ��� Rock�� �̿� üũ ��Ű��.(���߿� ������ ������)
	list<CObj*> pWallList = CObjMgr::Get_Instance()->Get_List()[OBJ_WALL];
	for (CObj* pObj : pWallList)
	{
		CRock* pRock = static_cast<CRock*>(pObj);
		pRock->Set_CheckNeighbor(true);
	}

}

int CRock::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CRock::Late_Update()
{
	//__super::Move_Frame();
	if (m_bCheckNeighbor)
	{
		Check_Neighbor();
		m_bCheckNeighbor = false;
	}
	
}

void CRock::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,			// ���� ���� DC
		m_tRect.left + iScrollX,	// ���� ���� ��ġ ��ǥ X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// ���� ���� �̹����� ����, ����
		(int)m_tInfo.fCY,
		hMemDC,						// ������ �̹��� DC	
		(int)m_tInfo.fCX * m_iRenderX,// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		(int)m_tInfo.fCY * m_iRenderY,
		(int)m_tInfo.fCX,			// ������ �̹����� ����, ����
		(int)m_tInfo.fCY,
		RGB_PURPLE);		// ������ ����
}

void CRock::Release()
{
}

void CRock::Check_Neighbor()
{

	POS tTopPos{ m_tInfo.fX, m_tInfo.fY - TILECY };
	POS tBottomPos{ m_tInfo.fX, m_tInfo.fY + TILECY };
	POS tLeftPos{ m_tInfo.fX - TILECX, m_tInfo.fY };
	POS tRightPos{ m_tInfo.fX + TILECX, m_tInfo.fY};

	//4��¥�� ���ڰ�
	if (CTileMgr::Get_Instance()->Get_TileObj(tBottomPos) &&
		CTileMgr::Get_Instance()->Get_TileObj(tLeftPos) &&
		CTileMgr::Get_Instance()->Get_TileObj(tRightPos)&&
		CTileMgr::Get_Instance()->Get_TileObj(tTopPos))
	{
		m_eCurState = CROSS;
		Change_Image();
		return;
	}

	//3�� ¥�� T �ø���
	//T12
	if (CTileMgr::Get_Instance()->Get_TileObj(tBottomPos) &&
		CTileMgr::Get_Instance()->Get_TileObj(tLeftPos) &&
		CTileMgr::Get_Instance()->Get_TileObj(tRightPos))
	{
		m_eCurState = T12;
		Change_Image();
		return;
	}

	//T3
	if (CTileMgr::Get_Instance()->Get_TileObj(tLeftPos) &&
		CTileMgr::Get_Instance()->Get_TileObj(tTopPos) &&
		CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
	{
		m_eCurState = T3;
		Change_Image();
		return;
	}

	//T6
	if (CTileMgr::Get_Instance()->Get_TileObj(tTopPos) &&
		CTileMgr::Get_Instance()->Get_TileObj(tLeftPos) &&
		CTileMgr::Get_Instance()->Get_TileObj(tRightPos))
	{
		m_eCurState = T6;
		Change_Image();
		return;
	}

	//T9
	if (CTileMgr::Get_Instance()->Get_TileObj(tRightPos) &&
		CTileMgr::Get_Instance()->Get_TileObj(tTopPos) &&
		CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
	{
		m_eCurState = T9;
		Change_Image();
		return;
	}

	//3�� ����------------------------------------------

	//1.���� �̿�
	if (CTileMgr::Get_Instance()->Get_TileObj(tLeftPos))
	{	
		//3.���ʵ� �̿�
		if (CTileMgr::Get_Instance()->Get_TileObj(tTopPos))
		{
			m_eCurState = RIGHT_BOTTOM;
			Change_Image();
			return;
		}
		//3.�Ʒ� �ʵ� �̿�
		else if (CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
		{
			m_eCurState = RIGHT_TOP;
			Change_Image();
			return;
		}
		//4.���� �� ���̴�.
		else if (!CTileMgr::Get_Instance()->Get_TileObj(tRightPos))
		{
			m_eCurState = END_RIGHT;
			Change_Image();
			return;
		}

		//5.����
		m_eCurState = HORIZONTAL;
		Change_Image();
		return;
	}
	//���� �� �̿�
	else if (CTileMgr::Get_Instance()->Get_TileObj(tRightPos))
	{
		
		//���ʵ� �̿�
		if (CTileMgr::Get_Instance()->Get_TileObj(tTopPos))
		{
			m_eCurState = LEFT_BOTTOM;
			Change_Image();
			return;
		}
		//�Ʒ� �ʵ� �̿�
		else if (CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
		{
			m_eCurState = LEFT_TOP;
			Change_Image();
			return;
		}


		//���ʿ� ���̴�
		else if (!CTileMgr::Get_Instance()->Get_TileObj(tLeftPos))
		{
			m_eCurState = END_LEFT;
			Change_Image();
			return;
		}

		m_eCurState = HORIZONTAL;
		Change_Image();
		return;
	}
	
	//vertical ���� �̿�
	else if (CTileMgr::Get_Instance()->Get_TileObj(tTopPos))
	{
		//3.���ʵ� �̿�
		if (CTileMgr::Get_Instance()->Get_TileObj(tLeftPos))
		{
			m_eCurState = RIGHT_BOTTOM;
			Change_Image();
			return;
		}
		//3.�����ʵ� �̿�
		else if (CTileMgr::Get_Instance()->Get_TileObj(tRightPos))
		{
			m_eCurState = LEFT_BOTTOM;
			Change_Image();
			return;
		}
		//�Ʒ� ���̴�
		if (!CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
		{
			m_eCurState = END_BOTTOM;
			Change_Image();
			return;
		}
		

		m_eCurState = VERTICAL;
		Change_Image();
		return;
	}
	//�Ʒ� �̿�
	else if (CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
	{
		//3.���ʵ� �̿�
		if (CTileMgr::Get_Instance()->Get_TileObj(tLeftPos))
		{
			m_eCurState = RIGHT_TOP;
			Change_Image();
			return;
		}
		//3.�����ʵ� �̿�
		else if (CTileMgr::Get_Instance()->Get_TileObj(tRightPos))
		{
			m_eCurState = LEFT_TOP;
			Change_Image();
			return;
		}
		//���ʿ� �ƹ��� ����.
		else if(!CTileMgr::Get_Instance()->Get_TileObj(tTopPos))
		{
			m_eCurState = END_TOP;
			Change_Image();
			return;
		}

		//����
		m_eCurState = VERTICAL;
		Change_Image();
		return;
	}
	//������ �ƹ��͵� ����.
	else
	{
		m_eCurState = SOLO;
		Change_Image();
		return;
	}
}
