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

	m_tInfo.fCX = 64.f; //나중에 오프셋 64(충돌범위)+16(렌더) 이런식으로 줘야할듯
    m_tInfo.fCY = 64.f;

	m_eCurState = SOLO;
	m_ePreState = SOLO;
	m_iRenderX = 0;
	m_iRenderY = 3;

	m_eRenderID = RENDER_GAMEOBJECT;

	m_bCheckNeighbor = true;

	//생성 됬을 때 일단 모든 Rock들 이웃 체크 시키자.(나중에 범위로 줄이자)
	list<CObj*> pWallList = CObjMgr::Get_Instance()->Get_List()[OBJ_WALL];
	for (CObj* pObj : pWallList)
	{
		CSteelWall* pRock = static_cast<CSteelWall*>(pObj);
		pRock->Set_CheckNeighbor(true);
	}

	//작업 변경사항을 림에게 알린다.
	CColonyMgr::Get_Instance()->Notify_TaskChange();

	//적들에게 길이 바뀌었을 수도 있음을 알리며, 네비게이션 종료
	//for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ENEMY])
	//{
	//	//static_cast<CPawn*>(pObj)->RequestNavStop();
	//	static_cast<CPawn*>(pObj)->Set_Navigating(false);//아닌거 같기도하고
	//	static_cast<CPawn*>(pObj)->Set_Navigating(true);
	//}

	//for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_RIM])
	//{
	//	static_cast<CPawn*>(pObj)->Set_Navigating(false);//아닌거 같기도하고
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

		//올라와 있던 타일을 통행가능하도록 만든다.
		CTileMgr::Get_Instance()->Set_TileOption(m_tInfo.fX, m_tInfo.fY, OPT_REACHABLE);
		//해당 타일위에 있는 Obj를  nullptr로 만든다.
		CTileMgr::Get_Instance()->Set_TileObj(m_tInfo.fX, m_tInfo.fY, nullptr);

		//철을 자기 위치에 생성
		CObj* pObj = CAbstractFactory<CSteel>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pObj);

		//생성되었을 때 타일에게 아이템이 있음을 알림
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

	////마우스 클릭 했을 때 타겟으로 설정
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

	//식민지 관리자가 해체모드일 경우 해체리스트에 넣는다. 
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

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		tPoint.x,//m_tRect.left + iScrollX -8,	// 복사 받을 위치 좌표 X, Y	
		tPoint.y,// + iScrollY -8,
		int(80 * CCamera::Get_Instance()->Get_Zoom()),			// 복사 받을 이미지의 가로, 세로
		int(80 * CCamera::Get_Instance()->Get_Zoom()),
		hMemDC,						// 복사할 이미지 DC	
		80 * m_iRenderX,// 비트맵 출력 시작 좌표(Left, top)
		80 * m_iRenderY,
		80,			// 복사할 이미지의 가로, 세로
		80,
		RGB_PURPLE);		// 제거할 색상

	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

	//작업 예약되었으면 해체 아이콘 출력
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

	////생성 됬을 때 일단 모든 Rock들 이웃 체크 시키자.(나중에 범위로 줄이자)
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
	// 주변 타일 위치 계산
	POS tTopPos{ (int)m_tInfo.fX, (int)m_tInfo.fY - TILECY };
	POS tBottomPos{ (int)m_tInfo.fX, (int)m_tInfo.fY + TILECY };
	POS tLeftPos{ (int)m_tInfo.fX - TILECX, (int)m_tInfo.fY };
	POS tRightPos{ (int)m_tInfo.fX + TILECX, (int)m_tInfo.fY };

	// TileMgr 인스턴스 가져오기
	CTileMgr* pTileMgr = CTileMgr::Get_Instance();

	// 주변 타일 상태 확인 (타일이 존재하고 OBJID가 OBJ_WALL인지 확인)
	bool hasTop = pTileMgr->Get_TileObj(tTopPos) && pTileMgr->Get_TileObj(tTopPos)->Get_ObjID() == OBJ_WALL;
	bool hasBottom = pTileMgr->Get_TileObj(tBottomPos) && pTileMgr->Get_TileObj(tBottomPos)->Get_ObjID() == OBJ_WALL;
	bool hasLeft = pTileMgr->Get_TileObj(tLeftPos) && pTileMgr->Get_TileObj(tLeftPos)->Get_ObjID() == OBJ_WALL;
	bool hasRight = pTileMgr->Get_TileObj(tRightPos) && pTileMgr->Get_TileObj(tRightPos)->Get_ObjID() == OBJ_WALL;

	// 4개 십자가
	if (hasTop && hasBottom && hasLeft && hasRight) {
		Set_State(CROSS);
		return;
	}

	// T자 형태
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

	// 왼쪽 이웃
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

	// 오른쪽 이웃
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

	// 위쪽 이웃
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

	// 아래쪽 이웃
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

	// 주변에 아무것도 없을 때
	Set_State(SOLO);
}

void CSteelWall::Set_State(STATE _eNewState) 
{
	m_eCurState = _eNewState;
	Change_Image();
}
