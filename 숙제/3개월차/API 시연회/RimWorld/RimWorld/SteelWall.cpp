#include "pch.h"
#include "SteelWall.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "ColonyMgr.h"
#include "Pawn.h"
#include "AbstractFactory.h"
#include "Steel.h"

CSteelWall::CSteelWall()
	:m_eCurState(END), m_ePreState(END), m_iRenderX(0), m_iRenderY(0), m_bCheckNeighbor(false),
	m_fDurability(0.f), m_fMaxDurability(0.f), m_bBrokendown(false)
{
}

CSteelWall::~CSteelWall()
{
    Release();
}

void CSteelWall::Change_Image()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CSteelWall::SOLO:
			m_iRenderX = 0;
			m_iRenderY = 3;
			break;
		case CSteelWall::HORIZONTAL:
			m_iRenderX = 2;
			m_iRenderY = 1;
			break;
		case CSteelWall::VERTICAL:
			m_iRenderX = 1;
			m_iRenderY = 2;
			break;
		case CSteelWall::END_LEFT:
			m_iRenderX = 2;
			m_iRenderY = 3;
			break;
		case CSteelWall::END_RIGHT:
			m_iRenderX = 0;
			m_iRenderY = 1;
			break;
		case CSteelWall::END_TOP:
			m_iRenderX = 0;
			m_iRenderY = 2;
			break;
		case CSteelWall::END_BOTTOM:
			m_iRenderX = 1;
			m_iRenderY = 3;
			break;

		case CSteelWall::RIGHT_BOTTOM:
			m_iRenderX = 1;
			m_iRenderY = 1;
			break;
		case CSteelWall::LEFT_BOTTOM:
			m_iRenderX = 3;
			m_iRenderY = 3;
			break;
		case CSteelWall::RIGHT_TOP:
			m_iRenderX = 0;
			m_iRenderY = 0;
			break;
		case CSteelWall::LEFT_TOP:
			m_iRenderX = 2;
			m_iRenderY = 2;
			break;

		case CSteelWall::T12:
			m_iRenderX = 2;
			m_iRenderY = 0;
			break;

		case CSteelWall::T3:
			m_iRenderX = 1;
			m_iRenderY = 0;
			break;

		case CSteelWall::T6:
			m_iRenderX = 3;
			m_iRenderY = 1;
			break;

		case CSteelWall::T9:
			m_iRenderX = 3;
			m_iRenderY = 2;
			break;

		case CSteelWall::CROSS:
			m_iRenderX = 3;
			m_iRenderY = 0;
			break;
		}

		m_ePreState = m_eCurState;
	}

}

void CSteelWall::Take_Damage(float _fDamage)
{
	m_fDurability -= _fDamage;

	//���� ó��
	if (m_fDurability <= 0.f)
	{
		m_bBrokendown = true;
		//Set_Destroyed(); //�ٷ� ���������� �����ؾ߰ڴ�.
	}
}

void CSteelWall::Initialize()
{
	Set_ImgKey(L"Wall_Atlas_Smooth");
	//m_pImgKey = L"Wall_Atlas_Smooth";

	m_tInfo.fCX = 64.f; //���߿� ������ 64(�浹����)+16(����) �̷������� ����ҵ�
    m_tInfo.fCY = 64.f;

	m_eCurState = SOLO;
	m_ePreState = SOLO;
	m_iRenderX = 0;
	m_iRenderY = 3;

	m_eRenderID = RENDER_GAMEOBJECT;

	m_bCheckNeighbor = true;

	m_fMaxDurability = 10.f;
	m_fDurability = m_fMaxDurability;

	//���� ���� �� �ϴ� ��� Rock�� �̿� üũ ��Ű��.(���߿� ������ ������)
	list<CObj*> pWallList = CObjMgr::Get_Instance()->Get_List()[OBJ_WALL];
	for (CObj* pObj : pWallList)
	{
		CSteelWall* pRock = static_cast<CSteelWall*>(pObj);
		pRock->Set_CheckNeighbor(true);
	}

	//�۾� ��������� ������ �˸���.
	CColonyMgr::Get_Instance()->Notify_TaskChange();

	//���鿡�� ���� �ٲ���� ���� ������ �˸���, �׺���̼� ����
	//for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ENEMY])
	//{
	//	//static_cast<CPawn*>(pObj)->RequestNavStop();
	//	static_cast<CPawn*>(pObj)->Set_Navigating(false);//�ƴѰ� ���⵵�ϰ�
	//	static_cast<CPawn*>(pObj)->Set_Navigating(true);
	//}

	//for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_RIM])
	//{
	//	static_cast<CPawn*>(pObj)->Set_Navigating(false);//�ƴѰ� ���⵵�ϰ�
	//	static_cast<CPawn*>(pObj)->Set_Navigating(true);
	//}


}

int CSteelWall::Update()
{
	if (m_bDestroyed)
	{
		return OBJ_DESTROYED;
	}

	if (m_bBrokendown)
	{
		Set_Destroyed();

		//�ö�� �ִ� Ÿ���� ���డ���ϵ��� �����.
		CTileMgr::Get_Instance()->Set_TileOption(m_tInfo.fX, m_tInfo.fY, OPT_REACHABLE);
		//�ش� Ÿ������ �ִ� Obj��  nullptr�� �����.
		CTileMgr::Get_Instance()->Set_TileObj(m_tInfo.fX, m_tInfo.fY, nullptr);

		//ö�� �ڱ� ��ġ�� ����
		CObj* pObj = CAbstractFactory<CSteel>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pObj);

		//�����Ǿ��� �� Ÿ�Ͽ��� �������� ������ �˸�
		//CTileMgr::Get_Instance()->Set_TileObj(m_tInfo.fX, m_tInfo.fY, pObj);

		//CColonyMgr::Get_Instance()->Get_DeconstructSet()->erase(this);
		set<TASK>& DeconstructSet = *CColonyMgr::Get_Instance()->Get_DeconstructSet();
		for (auto Iter = DeconstructSet.begin(); Iter != DeconstructSet.end();)
		{
			if ((*Iter).pObj == this)
			{
				Iter = DeconstructSet.erase(Iter);
			}
			else
			{
				++Iter;
			}
		}
	}

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CSteelWall::Late_Update()
{
	//__super::Move_Frame();
	if (m_bCheckNeighbor)
	{
		Check_Neighbor();
		m_bCheckNeighbor = false;
	}

	//���콺 Ŭ�� ���� �� Ÿ������ ����
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	ptMouse.x -= iScrollX;
	ptMouse.y -= iScrollY;

	if (PtInRect(&m_tRect, ptMouse))
	{
		//�Ĺ��� �����ڰ� ��ü����� ��� ��ü����Ʈ�� �ִ´�. 
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON) 
			&& CColonyMgr::Get_Instance()->Get_Mode() == CColonyMgr::MODE_DECONSTRUCT)
		{
			TASK tTask;
			tTask.pObj = this;
			CColonyMgr::Get_Instance()->Emplace_DeconstructSet(tTask);
			return;
		}

	}
	
}

void CSteelWall::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,			// ���� ���� DC
		m_tRect.left + iScrollX -8
		,	// ���� ���� ��ġ ��ǥ X, Y	
		m_tRect.top + iScrollY -8,
		80,			// ���� ���� �̹����� ����, ����
		80,
		hMemDC,						// ������ �̹��� DC	
		80 * m_iRenderX,// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		80 * m_iRenderY,
		80,			// ������ �̹����� ����, ����
		80,
		RGB_PURPLE);		// ������ ����

	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CSteelWall::Release()
{
	CColonyMgr::Get_Instance()->Notify_TaskChange();

	////���� ���� �� �ϴ� ��� Rock�� �̿� üũ ��Ű��.(���߿� ������ ������)
	//list<CObj*> pWallList = CObjMgr::Get_Instance()->Get_List()[OBJ_WALL];
	//for (CObj* pObj : pWallList)
	//{
	//	CSteelWall* pRock = static_cast<CSteelWall*>(pObj);
	//	pRock->Set_CheckNeighbor(true);
	//}
}

void CSteelWall::OnCollision(OBJID _eID, CObj* _pOther)
{
}

void CSteelWall::Check_Neighbor()
{

	POS tTopPos{ (int)m_tInfo.fX, (int)m_tInfo.fY - TILECY };
	POS tBottomPos{ (int)m_tInfo.fX, (int)m_tInfo.fY + TILECY };
	POS tLeftPos{ (int)m_tInfo.fX - TILECX, (int)m_tInfo.fY };
	POS tRightPos{ (int)m_tInfo.fX + TILECX, (int)m_tInfo.fY};

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
