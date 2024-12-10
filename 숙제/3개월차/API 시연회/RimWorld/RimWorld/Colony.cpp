#include "pch.h"
#include "Colony.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"

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

    CObj* pButton = CAbstractFactory<CMenuButton>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);
}

int CColony::Update()
{
    CObjMgr::Get_Instance()->Update();

    return 0;
}

void CColony::Late_Update()
{
    CObjMgr::Get_Instance()->Late_Update();
}

void CColony::Render(HDC hDC)
{
    // ���� ��� (Black �̹����� �ٷ� hDC�� �׸���)
    HDC hBlackDC = CBmpMgr::Get_Instance()->Find_Image(L"Black");
    BitBlt(hDC, 0, 0, WINCX, WINCY, hBlackDC, 0, 0, SRCCOPY);  // ���� ��� �׸���

    // �� Ÿ�� �׽�Ʈ (Ice �̹���)
    HDC hSnowDC = CBmpMgr::Get_Instance()->Find_Image(L"Ice");
    BitBlt(hDC, WINCX * 0.5, WINCY * 0.5f, 16, 16, hSnowDC, 0, 0, SRCCOPY);

    // ���� ��ư �׽�Ʈ (ButtonSubtleAtlas_Architect �̹���)
    HDC hArcDC = CBmpMgr::Get_Instance()->Find_Image(L"ButtonSubtleAtlas_Architect");
    BitBlt(hDC, 0, WINCY - 32, 128, 32, hArcDC, 0, 0, SRCCOPY);

    // ��ü �Ŵ��� ������
    CObjMgr::Get_Instance()->Render(hDC);
}

void CColony::Release()
{
    CObjMgr::Get_Instance()->Delete_ID(OBJ_BUTTON);
}
