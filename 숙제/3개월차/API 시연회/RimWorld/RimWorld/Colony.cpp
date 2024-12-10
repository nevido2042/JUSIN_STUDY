#include "pch.h"
#include "Colony.h"
#include "BmpMgr.h"

CColony::CColony()
{
}

CColony::~CColony()
{
    Release();
}

void CColony::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Terrain/Ice.bmp", L"Ice");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Architect.bmp", L"ButtonSubtleAtlas_Architect");
}

int CColony::Update()
{
    return 0;
}

void CColony::Late_Update()
{
}

void CColony::Render(HDC hDC)
{
    // 검은 배경
    HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Black");

    //눈 타일 테스트
    HDC hSnowDC = CBmpMgr::Get_Instance()->Find_Image(L"Ice");
    BitBlt(hMemDC,
        WINCX * 0.5, WINCY * 0.5f, 16, 16,
        hSnowDC,
        0,
        0,
        SRCCOPY);

    //구상 버튼 테스트
    HDC hArcDC = CBmpMgr::Get_Instance()->Find_Image(L"ButtonSubtleAtlas_Architect");
    BitBlt(hMemDC,
        0, WINCY-32, 128, 32,
        hArcDC,
        0,
        0,
        SRCCOPY);

    // 메모리 DC의 내용을 실제 화면에 출력 (더블 버퍼링)
    BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CColony::Release()
{
}
