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
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");
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
    HDC		hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground");

    BitBlt(hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);
}

void CColony::Release()
{
}
