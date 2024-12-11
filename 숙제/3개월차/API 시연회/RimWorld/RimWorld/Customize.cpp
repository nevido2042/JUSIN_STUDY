#include "pch.h"
#include "Customize.h"
#include "BmpMgr.h"

CCustomize::CCustomize()
{
}

CCustomize::~CCustomize()
{
	Release();
}

void CCustomize::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_south.bmp", L"Male_Average_Normal_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Male_south.bmp", L"Naked_Male_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Afro_south.bmp", L"Afro_south");
}

int CCustomize::Update()
{
	return 0;
}

int CCustomize::Late_Update()
{
    return 0;
}

void CCustomize::Render(HDC hDC)
{
    // ���� ��� �̹��� (Black �̹����� hDC�� �׸���)
    HDC hBlackDC = CBmpMgr::Get_Instance()->Find_Image(L"Black");
    BitBlt(hDC, 0, 0, WINCX, WINCY, hBlackDC, 0, 0, SRCCOPY);

    // ��� �̹��� (���� ���)
    HDC hBackgroundDC = CBmpMgr::Get_Instance()->Find_Image(L"SpaceBG");
    BitBlt(hDC, 0, 50, WINCX, WINCY, hBackgroundDC, 0, 0, SRCCOPY);

    // �� ����, ��, �Ӹ� �� �׽�Ʈ
    int iLeft = 350, iTop = 200;
    HDC hTestDC;
    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_south");
    GdiTransparentBlt(hDC, iLeft, iTop, 128, 128, hTestDC, 0, 0, 128, 128, RGB(163, 73, 164));

    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_south");
    GdiTransparentBlt(hDC, iLeft, iTop - HEAD_OFFSET, 128, 128, hTestDC, 0, 0, 128, 128, RGB(163, 73, 164));

    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Afro_south");
    GdiTransparentBlt(hDC, iLeft, iTop - HEAD_OFFSET, 128, 128, hTestDC, 0, 0, 128, 128, RGB(163, 73, 164));
}

void CCustomize::Release()
{
}
