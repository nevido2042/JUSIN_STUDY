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
		}

		m_ePreState = m_eCurState;
	}

}

void CRock::Initialize()
{
	Set_ImgKey(L"Rock_Atlas");
	//m_pImgKey = L"Rock_Atlas";

	m_tInfo.fCX = 80.f; //나중에 오프셋 64(충돌범위)+16(렌더) 이런식으로 줘야할듯
    m_tInfo.fCY = 80.f;

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

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_iRenderX,// 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * m_iRenderY,
		(int)m_tInfo.fCX,			// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB_PURPLE);		// 제거할 색상
}

void CRock::Release()
{
}

void CRock::Check_Neighbor()
{
	//solo 주변에 아무 것도 없다.

	POS tTopPos{ m_tInfo.fX, m_tInfo.fY - TILECY };
	POS tBottomPos{ m_tInfo.fX, m_tInfo.fY + TILECY };
	POS tLeftPos{ m_tInfo.fX - TILECX, m_tInfo.fY };
	POS tRightPos{ m_tInfo.fX + TILECX, m_tInfo.fY};

	//왼쪽 이웃
	if (CTileMgr::Get_Instance()->Get_TileObj(tLeftPos))
	{
		//위아래 다 이읏
		if (CTileMgr::Get_Instance()->Get_TileObj(tTopPos) && CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
		{
			m_eCurState = T3;
			Change_Image();
			return;
		}
		//위쪽도 이웃
		else if (CTileMgr::Get_Instance()->Get_TileObj(tTopPos))
		{
			m_eCurState = RIGHT_BOTTOM;
			Change_Image();
			return;
		}
		//아래 쪽도 이웃
		else if (CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
		{
			m_eCurState = RIGHT_TOP;
			Change_Image();
			return;
		}
		else if (!CTileMgr::Get_Instance()->Get_TileObj(tRightPos))
		{
			m_eCurState = END_RIGHT;
			Change_Image();
			return;
		}
		//왼쪽에 끝이다.
		else
		{
			m_eCurState = END_LEFT;
			Change_Image();
			return;
		}

		m_eCurState = HORIZONTAL;
		Change_Image();
		return;
	}
	//오른 쪽 이웃
	else if (CTileMgr::Get_Instance()->Get_TileObj(tRightPos))
	{
		//위아래 다 이읏
		if (CTileMgr::Get_Instance()->Get_TileObj(tTopPos) && CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
		{
			m_eCurState = T9;
			Change_Image();
			return;
		}
		//위쪽도 이웃
		else if (CTileMgr::Get_Instance()->Get_TileObj(tTopPos))
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
		else if (!CTileMgr::Get_Instance()->Get_TileObj(tRightPos))
		{
			m_eCurState = END_RIGHT;
			Change_Image();
			return;
		}
		//왼쪽에 끝이다.
		else
		{
			m_eCurState = END_LEFT;
			Change_Image();
			return;
		}

		m_eCurState = HORIZONTAL;
		Change_Image();
		return;
	}
	
	//vertical 수직 방향에 이웃이 있다.
	else if (CTileMgr::Get_Instance()->Get_TileObj(tTopPos)|| CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
	{
		//아래에 없다.
		if (!CTileMgr::Get_Instance()->Get_TileObj(tBottomPos))
		{
			m_eCurState = END_BOTTOM;
			Change_Image();
			return;
		}
		else
		{
			m_eCurState = END_TOP;
			Change_Image();
			return;
		}

		m_eCurState = VERTICAL;
		Change_Image();
		return;
	}

	else
	{
		m_eCurState = SOLO;
		Change_Image();
		return;
	}
}
