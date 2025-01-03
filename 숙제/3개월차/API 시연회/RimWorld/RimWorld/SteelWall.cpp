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

	//죽음 처리
	if (m_fDurability <= 0.f)
	{
		m_bBrokendown = true;
		//Set_Destroyed(); //바로 지우지말고 예약해야겠다.
	}
}

void CSteelWall::Initialize()
{
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

	m_fMaxDurability = 10.f;
	m_fDurability = m_fMaxDurability;

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
	if (m_bDestroyed)
	{
		return OBJ_DESTROYED;
	}

	if (m_bBrokendown)
	{
		Set_Destroyed();

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

	//마우스 클릭 했을 때 타겟으로 설정
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	ptMouse.x -= iScrollX;
	ptMouse.y -= iScrollY;

	if (PtInRect(&m_tRect, ptMouse))
	{
		//식민지 관리자가 해체모드일 경우 해체리스트에 넣는다. 
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

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX -8
		,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY -8,
		80,			// 복사 받을 이미지의 가로, 세로
		80,
		hMemDC,						// 복사할 이미지 DC	
		80 * m_iRenderX,// 비트맵 출력 시작 좌표(Left, top)
		80 * m_iRenderY,
		80,			// 복사할 이미지의 가로, 세로
		80,
		RGB_PURPLE);		// 제거할 색상

	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
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

void CSteelWall::Check_Neighbor()
{

	POS tTopPos{ (int)m_tInfo.fX, (int)m_tInfo.fY - TILECY };
	POS tBottomPos{ (int)m_tInfo.fX, (int)m_tInfo.fY + TILECY };
	POS tLeftPos{ (int)m_tInfo.fX - TILECX, (int)m_tInfo.fY };
	POS tRightPos{ (int)m_tInfo.fX + TILECX, (int)m_tInfo.fY};

	//4개짜리 십자가
	if (CTileMgr::Get_Instance()->Get_TileObj(tBottomPos) &&
		CTileMgr::Get_Instance()->Get_TileObj(tLeftPos) &&
		CTileMgr::Get_Instance()->Get_TileObj(tRightPos)&&
		CTileMgr::Get_Instance()->Get_TileObj(tTopPos))
	{
		m_eCurState = CROSS;
		Change_Image();
		return;
	}

	//3개 짜리 T 시리즈
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

	//3개 이하------------------------------------------

	//1.왼쪽 이웃
	if (CTileMgr::Get_Instance()->Get_TileObj(tLeftPos))
	{	
		//3.위쪽도 이웃
		if (CTileMgr::Get_Instance()->Get_TileObj(tTopPos))
		{
			m_eCurState = RIGHT_BOTTOM;
			Change_Image();
			return;
		}
		//3.아래 쪽도 이웃
		else if (CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
		{
			m_eCurState = RIGHT_TOP;
			Change_Image();
			return;
		}
		//4.오른 쪽 끝이다.
		else if (!CTileMgr::Get_Instance()->Get_TileObj(tRightPos))
		{
			m_eCurState = END_RIGHT;
			Change_Image();
			return;
		}

		//5.수평
		m_eCurState = HORIZONTAL;
		Change_Image();
		return;
	}
	//오른 쪽 이웃
	else if (CTileMgr::Get_Instance()->Get_TileObj(tRightPos))
	{
		
		//위쪽도 이웃
		if (CTileMgr::Get_Instance()->Get_TileObj(tTopPos))
		{
			m_eCurState = LEFT_BOTTOM;
			Change_Image();
			return;
		}
		//아래 쪽도 이웃
		else if (CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
		{
			m_eCurState = LEFT_TOP;
			Change_Image();
			return;
		}


		//왼쪽에 끝이다
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
	
	//vertical 위에 이웃
	else if (CTileMgr::Get_Instance()->Get_TileObj(tTopPos))
	{
		//3.왼쪽도 이웃
		if (CTileMgr::Get_Instance()->Get_TileObj(tLeftPos))
		{
			m_eCurState = RIGHT_BOTTOM;
			Change_Image();
			return;
		}
		//3.오른쪽도 이웃
		else if (CTileMgr::Get_Instance()->Get_TileObj(tRightPos))
		{
			m_eCurState = LEFT_BOTTOM;
			Change_Image();
			return;
		}
		//아래 끝이다
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
	//아래 이웃
	else if (CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
	{
		//3.왼쪽도 이웃
		if (CTileMgr::Get_Instance()->Get_TileObj(tLeftPos))
		{
			m_eCurState = RIGHT_TOP;
			Change_Image();
			return;
		}
		//3.오른쪽도 이웃
		else if (CTileMgr::Get_Instance()->Get_TileObj(tRightPos))
		{
			m_eCurState = LEFT_TOP;
			Change_Image();
			return;
		}
		//위쪽에 아무도 없다.
		else if(!CTileMgr::Get_Instance()->Get_TileObj(tTopPos))
		{
			m_eCurState = END_TOP;
			Change_Image();
			return;
		}

		//수직
		m_eCurState = VERTICAL;
		Change_Image();
		return;
	}
	//주위에 아무것도 없다.
	else
	{
		m_eCurState = SOLO;
		Change_Image();
		return;
	}
}
