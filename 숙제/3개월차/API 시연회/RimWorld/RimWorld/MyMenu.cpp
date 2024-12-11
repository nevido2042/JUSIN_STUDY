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
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BGPlanet.bmp", L"BGPlanet");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Start.bmp", L"Start");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Exit.bmp", L"Exit");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/GameTitle.bmp", L"GameTitle");

    CObj* pButton = CAbstractFactory<CMyButton>::Create(WINCX - 200.f, 325.f);
    pButton->Set_ImgKey(L"Start");
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);

    pButton = CAbstractFactory<CMyButton>::Create(WINCX - 200.f, 400.f);
    pButton->Set_ImgKey(L"Exit");
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);
}

int CMyMenu::Update()
{
    CObjMgr::Get_Instance()->Update();

    return OBJ_NOEVENT;
}

int CMyMenu::Late_Update()
{
    CObjMgr::Get_Instance()->Late_Update();
    return OBJ_NOEVENT;
}

void CMyMenu::Render(HDC hDC)
{
    // ���� ��� �̹��� (Black �̹����� hDC�� �׸���)
    HDC hBlackDC = CBmpMgr::Get_Instance()->Find_Image(L"Black");
    BitBlt(hDC, 0, 0, WINCX, WINCY, hBlackDC, 0, 0, SRCCOPY);

    // ��� �̹��� (���� ���)
    HDC hBackgroundDC = CBmpMgr::Get_Instance()->Find_Image(L"BGPlanet");
    BitBlt(hDC, 0, 0, WINCX, WINCY, hBackgroundDC, 0, 0, SRCCOPY);

    // ���� Ÿ��Ʋ �̹��� (���� ��� ���)
    int iCX(1024), iCY(145);
    int iLeft = (WINCX - iCX) * 0.5f, iTop = iCY;
    HDC hGameTitleDC = CBmpMgr::Get_Instance()->Find_Image(L"GameTitle");
    GdiTransparentBlt(hDC, iLeft, iTop, iCX, iCY, hGameTitleDC, 0, 0, iCX, iCY, RGB_PURPLE);

    // ��ü �Ŵ��� ������
    CObjMgr::Get_Instance()->Render(hDC);

}

void CMyMenu::Release()
{
    CObjMgr::Get_Instance()->Delete_ID(OBJ_BUTTON);
}
