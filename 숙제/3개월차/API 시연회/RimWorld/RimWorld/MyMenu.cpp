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
    //검은색 뒷배경
    HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);
    Rectangle(hDC, 0, 0, WINCX, WINCY);
    SelectObject(hDC, oldBrush);
    DeleteObject(myBrush);

    HDC		hMemDC;
    //우주 배경
    hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Menu");
    BitBlt(hDC,
        0, 50, WINCX, WINCY,
        hMemDC,
        0,
        0,
        SRCCOPY);

    //게임 타이틀
    int iLeft(350), iTop(200);
    /*hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"GameTitle");
    BitBlt(hDC,
        iLeft, iTop, iLeft + 400, iTop + 57,
        hMemDC,
        0,
        0,
        SRCCOPY);*/
    hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"GameTitle");
    GdiTransparentBlt(hDC,                      // 복사 받을 DC
        iLeft, iTop,                            // 복사 받을 위치 좌표 X, Y
        400, 57,                                // 복사 받을 이미지의 가로, 세로
        hMemDC,                                 // 복사할 이미지 DC	
        0,                                      // 비트맵 출력 시작 좌표(Left, top)
        0,
        400,                                    // 복사할 이미지의 가로, 세로
        57,
        RGB(255, 255, 255));                    // 제거할 색상

    CObjMgr::Get_Instance()->Render(hDC);
}

void CMyMenu::Release()
{
    CObjMgr::Get_Instance()->Delete_ID(OBJ_BUTTON);
}
