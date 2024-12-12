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
    pObj = CAbstractFactory<CRim>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_RIM, pObj);
    pObj = CAbstractFactory<CRock>::Create(64.f, 64.f);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
}

int CColony::Update()
{
    Input_Key();

    CObjMgr::Get_Instance()->Update();

    CTileMgr::Get_Instance()->Update();

    return OBJ_NOEVENT;
}

void CColony::Late_Update()
{
    CObjMgr::Get_Instance()->Late_Update();

    CTileMgr::Get_Instance()->Late_Update();

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
}

void CColony::Input_Key()
{
    //화면 줌아웃(RimWorld.cpp 이벤트 부분에 있음)

    //화면 스크롤
    float fSpeed(10.f);
    if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
    {
        CScrollMgr::Get_Instance()->Set_ScrollX(fSpeed);
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
    {
        CScrollMgr::Get_Instance()->Set_ScrollX(-fSpeed);
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
    {
        CScrollMgr::Get_Instance()->Set_ScrollY(fSpeed);
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
    {
        CScrollMgr::Get_Instance()->Set_ScrollY(-fSpeed);
    }
}
