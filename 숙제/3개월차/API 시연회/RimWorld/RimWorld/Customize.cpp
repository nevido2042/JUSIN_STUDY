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
    // 검은 배경 이미지 (Black 이미지를 hDC에 그리기)
    HDC hBlackDC = CBmpMgr::Get_Instance()->Find_Image(L"Black");
    BitBlt(hDC, 0, 0, WINCX, WINCY, hBlackDC, 0, 0, SRCCOPY);

    // 배경 이미지 (우주 배경)
    HDC hBackgroundDC = CBmpMgr::Get_Instance()->Find_Image(L"SpaceBG");
    BitBlt(hDC, 0, 50, WINCX, WINCY, hBackgroundDC, 0, 0, SRCCOPY);

    // 림 몸통, 얼굴, 머리 순 테스트
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
