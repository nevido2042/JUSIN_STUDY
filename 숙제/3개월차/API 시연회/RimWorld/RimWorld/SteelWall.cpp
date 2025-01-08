#include "pch.h"
#include "SteelWall.h"
#include "BmpMgr.h"

#include "TileMgr.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "ColonyMgr.h"
#include "Pawn.h"
#include "AbstractFactory.h"
#include "Steel.h"
#include "SoundMgr.h"
#include "TutorialMgr.h"
#include "Camera.h"

CSteelWall::CSteelWall()
	:m_eCurState(END), m_ePreState(END), m_iRenderX(0), m_iRenderY(0), m_bCheckNeighbor(false)
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

void CSteelWall::Initialize()
{
	CBreakable::Initialize();

	Set_ObjID(OBJ_WALL);

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
	if (m_bBrokendown)
	{
		if (CTutorialMgr::Get_Instance()->Get_CurQuest() == CTutorialMgr::QUEST_DECONSTRUCT)
		{
			CTutorialMgr::Get_Instance()->IncreaseQuestProgress();
		}

		CSoundMgr::Get_Instance()->StopSound(SOUND_WALL);
		CSoundMgr::Get_Instance()->PlaySound(L"StoneBlock_Drop_1a.wav", SOUND_WALL, .5f);
		//Set_Destroyed();

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

	return CBreakable::Update();
}

void CSteelWall::Late_Update()
{
	//__super::Move_Frame();
	if (m_bCheckNeighbor)
	{
		Check_Neighbor();
		m_bCheckNeighbor = false;
	}

	////���콺 Ŭ�� ���� �� Ÿ������ ����
	//POINT	ptMouse{};

	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);

	//int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//ptMouse.x -= iScrollX;
	//ptMouse.y -= iScrollY;

	//if (PtInRect(&m_tRect, ptMouse))
	//{
	//	

	//}

	//�Ĺ��� �����ڰ� ��ü����� ��� ��ü����Ʈ�� �ִ´�. 
	if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON)
		&& CColonyMgr::Get_Instance()->Get_Mode() == CColonyMgr::MODE_DECONSTRUCT)
	{
		if (Is_MouseHovered_Scrolled())
		{
			TASK tTask;
			tTask.pObj = this;
			CColonyMgr::Get_Instance()->Emplace_DeconstructSet(tTask);
			//return;
		}
	}
	
}

void CSteelWall::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	POINT tPoint = CCamera::Get_Instance()->WorldToScreen((float)(m_tRect.left - 8), (float)(m_tRect.top - 8));

	GdiTransparentBlt(hDC,			// ���� ���� DC
		tPoint.x,//m_tRect.left + iScrollX -8,	// ���� ���� ��ġ ��ǥ X, Y	
		tPoint.y,// + iScrollY -8,
		int(80 * CCamera::Get_Instance()->Get_Zoom()),			// ���� ���� �̹����� ����, ����
		int(80 * CCamera::Get_Instance()->Get_Zoom()),
		hMemDC,						// ������ �̹��� DC	
		80 * m_iRenderX,// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		80 * m_iRenderY,
		80,			// ������ �̹����� ����, ����
		80,
		RGB_PURPLE);		// ������ ����

	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

	//�۾� ����Ǿ����� ��ü ������ ���
	if (m_bTaskReserved)
	{
		HDC		hDeconstructDC = CBmpMgr::Get_Instance()->Find_Image(L"Deconstruct_mini");

		float fZoom = CCamera::Get_Instance()->Get_Zoom();
		POINT tPoint = CCamera::Get_Instance()->WorldToScreen(
			(float)Get_Rect()->left,
			(float)Get_Rect()->top
		);

		GdiTransparentBlt(hDC,
			(int)tPoint.x,
			(int)tPoint.y,
			(int)(64 * fZoom),
			(int)(64 * fZoom),
			hDeconstructDC,
			0, 0,
			64,
			64,
			RGB_WHITE);
	}
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

void CSteelWall::Check_Neighbor() {
	// �ֺ� Ÿ�� ��ġ ���
	POS tTopPos{ (int)m_tInfo.fX, (int)m_tInfo.fY - TILECY };
	POS tBottomPos{ (int)m_tInfo.fX, (int)m_tInfo.fY + TILECY };
	POS tLeftPos{ (int)m_tInfo.fX - TILECX, (int)m_tInfo.fY };
	POS tRightPos{ (int)m_tInfo.fX + TILECX, (int)m_tInfo.fY };

	// TileMgr �ν��Ͻ� ��������
	CTileMgr* pTileMgr = CTileMgr::Get_Instance();

	// �ֺ� Ÿ�� ���� Ȯ�� (Ÿ���� �����ϰ� OBJID�� OBJ_WALL���� Ȯ��)
	bool hasTop = pTileMgr->Get_TileObj(tTopPos) && pTileMgr->Get_TileObj(tTopPos)->Get_ObjID() == OBJ_WALL;
	bool hasBottom = pTileMgr->Get_TileObj(tBottomPos) && pTileMgr->Get_TileObj(tBottomPos)->Get_ObjID() == OBJ_WALL;
	bool hasLeft = pTileMgr->Get_TileObj(tLeftPos) && pTileMgr->Get_TileObj(tLeftPos)->Get_ObjID() == OBJ_WALL;
	bool hasRight = pTileMgr->Get_TileObj(tRightPos) && pTileMgr->Get_TileObj(tRightPos)->Get_ObjID() == OBJ_WALL;

	// 4�� ���ڰ�
	if (hasTop && hasBottom && hasLeft && hasRight) {
		Set_State(CROSS);
		return;
	}

	// T�� ����
	if (hasBottom && hasLeft && hasRight) {
		Set_State(T12);
		return;
	}
	if (hasTop && hasLeft && hasBottom) {
		Set_State(T3);
		return;
	}
	if (hasTop && hasLeft && hasRight) {
		Set_State(T6);
		return;
	}
	if (hasRight && hasTop && hasBottom) {
		Set_State(T9);
		return;
	}

	// ���� �̿�
	if (hasLeft) {
		if (hasTop) {
			Set_State(RIGHT_BOTTOM);
		}
		else if (hasBottom) {
			Set_State(RIGHT_TOP);
		}
		else if (!hasRight) {
			Set_State(END_RIGHT);
		}
		else {
			Set_State(HORIZONTAL);
		}
		return;
	}

	// ������ �̿�
	if (hasRight) {
		if (hasTop) {
			Set_State(LEFT_BOTTOM);
		}
		else if (hasBottom) {
			Set_State(LEFT_TOP);
		}
		else if (!hasLeft) {
			Set_State(END_LEFT);
		}
		else {
			Set_State(HORIZONTAL);
		}
		return;
	}

	// ���� �̿�
	if (hasTop) {
		if (hasLeft) {
			Set_State(RIGHT_BOTTOM);
		}
		else if (hasRight) {
			Set_State(LEFT_BOTTOM);
		}
		else if (!hasBottom) {
			Set_State(END_BOTTOM);
		}
		else {
			Set_State(VERTICAL);
		}
		return;
	}

	// �Ʒ��� �̿�
	if (hasBottom) {
		if (hasLeft) {
			Set_State(RIGHT_TOP);
		}
		else if (hasRight) {
			Set_State(LEFT_TOP);
		}
		else if (!hasTop) {
			Set_State(END_TOP);
		}
		else {
			Set_State(VERTICAL);
		}
		return;
	}

	// �ֺ��� �ƹ��͵� ���� ��
	Set_State(SOLO);
}

void CSteelWall::Set_State(STATE _eNewState) 
{
	m_eCurState = _eNewState;
	Change_Image();
}
