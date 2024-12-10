#include "pch.h"
#include "MyMenu.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"

CMyMenu::CMyMenu()
{
}

CMyMenu::~CMyMenu()
{
    Release();
}

void CMyMenu::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Black.bmp", L"Black");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/Menu.bmp", L"Menu");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Start.bmp", L"Start");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Exit.bmp", L"Exit");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/GameTitle.bmp", L"GameTitle");

    CObj* pButton = CAbstractFactory<CMyButton>::Create(550.f, 325.f);
    pButton->Set_ImgKey(L"Start");
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);

    pButton = CAbstractFactory<CMyButton>::Create(550.f, 400.f);
    pButton->Set_ImgKey(L"Exit");
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);
}

int CMyMenu::Update()
{
    CObjMgr::Get_Instance()->Update();

    return 0;
}

void CMyMenu::Late_Update()
{
    CObjMgr::Get_Instance()->Late_Update();
}

void CMyMenu::Render(HDC hDC)
{
    // 검은 배경
    HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Black");

    // 배경 이미지 (우주 배경)
    HDC hBackgroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Menu");
    BitBlt(hMemDC, 0, 50, WINCX, WINCY, hBackgroundDC, 0, 0, SRCCOPY);

    // 게임 타이틀 이미지 (투명 배경 사용)
    int iLeft = 350, iTop = 200;
    HDC hGameTitleDC = CBmpMgr::Get_Instance()->Find_Image(L"GameTitle");
    GdiTransparentBlt(hMemDC, iLeft, iTop, 400, 57, hGameTitleDC, 0, 0, 400, 57, RGB(255, 255, 255));

    // 객체 매니저 렌더링
    CObjMgr::Get_Instance()->Render(hMemDC);

    // 메모리 DC의 내용을 실제 화면에 출력 (더블 버퍼링)
    BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

}

void CMyMenu::Release()
{
    CObjMgr::Get_Instance()->Delete_ID(OBJ_BUTTON);
}
