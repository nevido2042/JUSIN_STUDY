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
    // ���� ���
    HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Black");

    //�� Ÿ�� �׽�Ʈ
    HDC hSnowDC = CBmpMgr::Get_Instance()->Find_Image(L"Ice");
    BitBlt(hMemDC,
        WINCX * 0.5, WINCY * 0.5f, 16, 16,
        hSnowDC,
        0,
        0,
        SRCCOPY);

    //���� ��ư �׽�Ʈ
    HDC hArcDC = CBmpMgr::Get_Instance()->Find_Image(L"ButtonSubtleAtlas_Architect");
    BitBlt(hMemDC,
        0, WINCY-32, 128, 32,
        hArcDC,
        0,
        0,
        SRCCOPY);

    // �޸� DC�� ������ ���� ȭ�鿡 ��� (���� ���۸�)
    BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CColony::Release()
{
}
