#include "pch.h"
#include "CMyEdit.h"
#include "CTileMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"

CMyEdit::CMyEdit()
{
}

CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::Initialize()
{
	CTileMgr::Get_Instance()->Initialize();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Tile.bmp", L"Tile");
}

int CMyEdit::Update()
{
	CTileMgr::Get_Instance()->Update();

	return 0;
}

void CMyEdit::Late_Update()
{
	Key_Input();
	CTileMgr::Get_Instance()->Late_Update();

}

void CMyEdit::Render(HDC hDC)
{
	CTileMgr::Get_Instance()->Render(hDC);
}

void CMyEdit::Release()
{
}

void CMyEdit::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		CScrollMgr::Get_Instance()->Set_ScrollY(5.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		ptMouse.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

		CTileMgr::Get_Instance()->Picking_Tile(ptMouse, 1, 1);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		CTileMgr::Get_Instance()->Save_Tile();
		return;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('L'))
	{
		CTileMgr::Get_Instance()->Load_Tile();
		return;
	}

}
