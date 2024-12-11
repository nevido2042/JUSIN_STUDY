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
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Terrain/Ice.bmp", L"Ice");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Architect.bmp", L"ButtonSubtleAtlas_Architect");
    //Rim
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_south.bmp", L"Male_Average_Normal_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Male_south.bmp", L"Naked_Male_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Afro_south.bmp", L"Afro_south");

    CObj* pButton = CAbstractFactory<CMenuButton>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);

    CTileMgr::Get_Instance()->Initialize();
    //CTileMgr::Get_Instance()->Create_TileMap();

    CObj* pRim = CAbstractFactory<CRim>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_RIM, pRim);
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
    // ���� ��� (Black �̹����� �ٷ� hDC�� �׸���)
    HDC hBlackDC = CBmpMgr::Get_Instance()->Find_Image(L"Black");
    BitBlt(hDC, 0, 0, WINCX, WINCY, hBlackDC, 0, 0, SRCCOPY);  // ���� ��� �׸���

    CTileMgr::Get_Instance()->Render(hDC);

    // ��ü �Ŵ��� ������
    CObjMgr::Get_Instance()->Render(hDC);
}

void CColony::Release()
{
    CObjMgr::Get_Instance()->Delete_ID(OBJ_BUTTON);
    CTileMgr::Destroy_Instance();
}

void CColony::Input_Key()
{
    //ȭ�� �ܾƿ�(RimWorld.cpp �̺�Ʈ �κп� ����)

    //ȭ�� ��ũ��
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
}
