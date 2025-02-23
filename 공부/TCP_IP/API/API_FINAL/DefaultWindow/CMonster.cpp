#include "pch.h"
#include "CMonster.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CMonster::CMonster()
{
    ZeroMemory(m_tPoint, sizeof(m_tPoint));
}

CMonster::~CMonster()
{
    Release();
}

void CMonster::Initialize()
{
    m_tInfo.fCX = 300.f;
    m_tInfo.fCY = 300.f;
    m_fSpeed = 3.f;

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Monster.bmp", L"Monster");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Plg.bmp", L"Plg");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Reset.bmp", L"Reset");

    m_pImgKey = L"Monster";

    m_eRender = RENDER_GAMEOBJECT;
}

int CMonster::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

   
    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CMonster::Late_Update()
{
    float       fDiagonal = sqrt((m_tInfo.fCX / 2.f) * (m_tInfo.fCX / 2.f) + (m_tInfo.fCY / 2.f) * (m_tInfo.fCY / 2.f));

    // 좌 상단
    m_tPoint[0].x = LONG((m_tInfo.fCX / 2.f) + (fDiagonal * cosf((m_fAngle + 135.f) * PI / 180.f)));
    m_tPoint[0].y = LONG((m_tInfo.fCY / 2.f) - (fDiagonal * sinf((m_fAngle + 135.f) * PI / 180.f)));

    // 우 상단
    m_tPoint[1].x = LONG((m_tInfo.fCX / 2.f) + (fDiagonal * cosf((m_fAngle + 45.f) * PI / 180.f)));
    m_tPoint[1].y = LONG((m_tInfo.fCY / 2.f) - (fDiagonal * sinf((m_fAngle + 45.f) * PI / 180.f)));

    // 좌 하단
    m_tPoint[2].x = LONG((m_tInfo.fCX / 2.f) + (fDiagonal * cosf((m_fAngle + 225.f) * PI / 180.f)));
    m_tPoint[2].y = LONG((m_tInfo.fCY / 2.f) - (fDiagonal * sinf((m_fAngle + 225.f) * PI / 180.f)));


    if (GetAsyncKeyState(VK_LBUTTON))
    {
        m_fAngle += 5.f;
    }

    if (GetAsyncKeyState(VK_RBUTTON))
    {
        m_fAngle -= 5.f;
    }

}

void CMonster::Render(HDC hDC)
{
    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    HDC		hPlgDC = CBmpMgr::Get_Instance()->Find_Image(L"Plg");
    HDC		hResetDC = CBmpMgr::Get_Instance()->Find_Image(L"Reset");

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // 1. 회전을 시켜 그린다.

    PlgBlt(hPlgDC,      // 회전 시킨 이미지를 어디에 그릴 것인가
        m_tPoint,       // 그림을 그릴 사각형의 좌표 중 POINT 변수로 3개(좌상단, 우상단, 좌하단 순서)
        hMemDC,         // 어디에 있는 이미지를 회전시켜 그릴 것인가
        0, 0,           // 회전 시켜 그릴 이미지의 시작 좌표
        (int)m_tInfo.fCX,   // 회전 시켜 그릴 이미지의 가로, 세로 사이즈
        (int)m_tInfo.fCY,
        NULL, NULL, NULL);      // 각종 마스킹 텍스처 대충 NULL


    // 2. 불필요한 픽셀 색상을 제거하여 그린다.
    GdiTransparentBlt(hDC,			// 복사 받을 DC
        m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
        m_tRect.top + iScrollY,
        (int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
        (int)m_tInfo.fCY,
        hPlgDC,						// 복사할 이미지 DC	
        0,							// 비트맵 출력 시작 좌표(Left, top)
        0,
        (int)m_tInfo.fCX,			// 복사할 이미지의 가로, 세로
        (int)m_tInfo.fCY,
        RGB(255, 255, 255));		// 제거할 색상

    // 3. 다음 프레임을 위해 민무늬 이미지로 덮어 그린다.

    BitBlt(hPlgDC, 
        0, 0,
        (int)m_tInfo.fCX,
        (int)m_tInfo.fCY,
        hResetDC, 0, 0, SRCCOPY);
}

void CMonster::Release()
{
}
