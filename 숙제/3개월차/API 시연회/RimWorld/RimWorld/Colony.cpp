#include "pch.h"
#include "Colony.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "TileMgr.h"

CColony::CColony()
{
}

CColony::~CColony()
{
    Release();
}

void CColony::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Menu.bmp", L"ButtonSubtleAtlas_Menu");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Terrain/Ice.bmp", L"Ice");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Architect.bmp", L"ButtonSubtleAtlas_Architect");

    //SetScene 문제로 잠시 보류
    /*CObj* pButton = CAbstractFactory<CMenuButton>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);*/
    CTileMgr::Get_Instance()->Create_TileMap();

    /*CObj* pTile = CAbstractFactory<CTile>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_MOUSE, pTile);*/
}

int CColony::Update()
{
    CObjMgr::Get_Instance()->Update();

    if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
    {
        CScrollMgr::Get_Instance()->Set_ScrollX(5.f);
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
    {
        CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
    {
        CScrollMgr::Get_Instance()->Set_ScrollY(5.f);
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
    {
        CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);
    }

    return 0;
}

void CColony::Late_Update()
{
    CObjMgr::Get_Instance()->Late_Update();
}

void CColony::Render(HDC hDC)
{
    // 검은 배경 (Black 이미지를 바로 hDC에 그리기)
    HDC hBlackDC = CBmpMgr::Get_Instance()->Find_Image(L"Black");
    BitBlt(hDC, 0, 0, WINCX, WINCY, hBlackDC, 0, 0, SRCCOPY);  // 검은 배경 그리기

    //// 눈 타일 테스트 (Ice 이미지)
    //HDC hSnowDC = CBmpMgr::Get_Instance()->Find_Image(L"Ice");
    //BitBlt(hDC, WINCX * 0.5, WINCY * 0.5f, 16, 16, hSnowDC, 0, 0, SRCCOPY);

    //// 구상 버튼 테스트 (ButtonSubtleAtlas_Architect 이미지)
    //HDC hArcDC = CBmpMgr::Get_Instance()->Find_Image(L"ButtonSubtleAtlas_Architect");
    //BitBlt(hDC, 0, WINCY - 32, 128, 32, hArcDC, 0, 0, SRCCOPY);

    // 객체 매니저 렌더링
    CObjMgr::Get_Instance()->Render(hDC);
}

void CColony::Release()
{
    CObjMgr::Get_Instance()->Delete_ID(OBJ_BUTTON);
    CTileMgr::Destroy_Instance();
}
