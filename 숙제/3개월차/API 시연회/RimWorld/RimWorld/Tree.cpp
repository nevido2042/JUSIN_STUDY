#include "pch.h"
#include "Tree.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CTree::CTree()
{
}

CTree::~CTree()
{
    Release();
}

void CTree::Initialize()
{
	Set_ObjID(OBJ_TREE);


	Set_ImgKey(L"TreePoplarA");

    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_GAMEOBJECT;
}

int CTree::Update()
{
    if (m_bDestroyed)
        return OBJ_DESTROYED;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CTree::Late_Update()
{
}

void CTree::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX - IMAGE_OFFSET_X,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY - IMAGE_OFFSET_Y - 32,
		128,			// 복사 받을 이미지의 가로, 세로
		128,
		hMemDC,						// 복사할 이미지 DC	
		0,				// 비트맵 출력 시작 좌표(Left, top)
		0,
		128,			// 복사할 이미지의 가로, 세로
		128,
		RGB_PURPLE);		// 제거할 색상
}

void CTree::Release()
{
}
