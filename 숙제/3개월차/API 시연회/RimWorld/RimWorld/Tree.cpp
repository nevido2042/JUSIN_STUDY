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

	GdiTransparentBlt(hDC,			// ���� ���� DC
		m_tRect.left + iScrollX - IMAGE_OFFSET_X,	// ���� ���� ��ġ ��ǥ X, Y	
		m_tRect.top + iScrollY - IMAGE_OFFSET_Y - 32,
		128,			// ���� ���� �̹����� ����, ����
		128,
		hMemDC,						// ������ �̹��� DC	
		0,				// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		0,
		128,			// ������ �̹����� ����, ����
		128,
		RGB_PURPLE);		// ������ ����
}

void CTree::Release()
{
}
