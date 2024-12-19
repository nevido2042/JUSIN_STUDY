#include "pch.h"
#include "Logo.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

CLogo::CLogo()
{
}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Logo/Logo.bmp", L"Logo");


}

int CLogo::Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN)||
		CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		CSceneMgr::Get_Instance()->Set_Scene(SC_MENU);
		return OBJ_NOEVENT;
	}


	return OBJ_NOEVENT;
}

void CLogo::Late_Update()
{

}

void CLogo::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Logo");

	BitBlt(hDC,
		int((WINCX - 800) * 0.5f), int((WINCY - 600) * 0.5f), WINCX, WINCY,
		hMemDC,
		0,
		0,
		SRCCOPY);
}

void CLogo::Release()
{

}
