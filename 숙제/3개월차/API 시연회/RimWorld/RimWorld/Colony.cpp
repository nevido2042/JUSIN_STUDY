#include "pch.h"
#include "Colony.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "TileMgr.h"
#include "PathFinder.h"

CColony::CColony()
{
}

CColony::~CColony()
{
    Release();
}

void CColony::Initialize()
{
    //UI
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Menu.bmp", L"ButtonSubtleAtlas_Menu");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Architect.bmp", L"ButtonSubtleAtlas_Architect");
    //Rim
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_south.bmp", L"Male_Average_Normal_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Male_south.bmp", L"Naked_Male_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Afro_south.bmp", L"Afro_south");
    //OBJ
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Terrain/Ice.bmp", L"Ice");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Building/Linked/Rock_Atlas.bmp", L"Rock_Atlas");


    CObj* pObj(nullptr);
    //UI
    pObj = CAbstractFactory<CMenuBtn>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pObj);
    pObj = CAbstractFactory<CArcitectBtn>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pObj);

    //맵
    CTileMgr::Get_Instance()->Initialize();

    //오브젝트
    pObj = CAbstractFactory<CRim>::Create(32, 32);
    CObjMgr::Get_Instance()->Add_Object(OBJ_RIM, pObj);

    for (int i = 0; i < 100; ++i)
    {
        int iX = int(rand() % TILEX);
        int iY = int(rand() % TILEY);

        pObj = CAbstractFactory<CRock>::Create(64 * iX + 32, 64 * iY + 32);
        CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
        CTileMgr::Get_Instance()->Set_TileOption(64 * iX + 32, 64 * iY + 32, OPT_BLOCKED);
    }

    /*for (int i = 0; i < 5; ++i)
    {
        pObj = CAbstractFactory<CRock>::Create(64 * i + 32, 64 * i + 32);
        CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
        CTileMgr::Get_Instance()->Set_TileOption(64 * i + 32, 64 * i + 32, OPT_BLOCKED);
    }*/


}

int CColony::Update()
{
    CColonyMgr::Get_Instance()->Update();

    CObjMgr::Get_Instance()->Update();

    CTileMgr::Get_Instance()->Update();

   

    return OBJ_NOEVENT;
}

void CColony::Late_Update()
{
    CObjMgr::Get_Instance()->Late_Update();

    CTileMgr::Get_Instance()->Late_Update();

    CColonyMgr::Get_Instance()->Late_Update();
}

void CColony::Render(HDC hDC)
{
    // 검은 배경 (Black 이미지를 바로 hDC에 그리기)
    HDC hBlackDC = CBmpMgr::Get_Instance()->Find_Image(L"Black");
    BitBlt(hDC, 0, 0, WINCX, WINCY, hBlackDC, 0, 0, SRCCOPY);  // 검은 배경 그리기

    CTileMgr::Get_Instance()->Render(hDC);

    // 객체 매니저 렌더링
    CObjMgr::Get_Instance()->Render(hDC);
}

void CColony::Release()
{
    CObjMgr::Get_Instance()->Delete_ID(OBJ_BUTTON);
    CTileMgr::Destroy_Instance();
    CColonyMgr::Destroy_Instance();
    CPathFinder::Destroy_Instance();
}
