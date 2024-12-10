#include "pch.h"
#include "CMonster_1.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CMonster_1::CMonster_1()
{
}

CMonster_1::~CMonster_1()
{
}

void CMonster_1::Initialize()
{
    fGravity = 3.f;
	m_fSpeed = 3.f;
	m_tInfo = { 300.f,300,50,50 };
    m_iHp = 10;

	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Fire_Monster1");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Fire_Monster2");
}

int CMonster_1::Update()
{
   // 중력대신..
    m_tInfo.fY += fGravity;
    if (780 > m_tInfo.fY && 522 < m_tInfo.fY)
        fGravity = 0.f;

    m_tInfo.fX -= m_fSpeed; 
  
    if (m_tInfo.fX > 800 || m_tInfo.fX < 100)
        m_fSpeed *= -1.f; 

    // 죽었을 경우 처리
    if (m_bDead || m_iHp <= 0)
        return OBJ_DEAD;

    // 사각형 갱신
    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CMonster_1::Late_Update()
{
}

void CMonster_1::Render(HDC hDC)
{
    static ULONGLONG Ulllast_SwitchTime = 0; // 마지막으로 이미지가 변경된 시간

    static bool bFirstImage = true;    // 현재 표시 중인 이미지 

    ULONGLONG ullCurrentTime = GetTickCount64();
    if (ullCurrentTime - Ulllast_SwitchTime > 400) //  0.4초
    {
        bFirstImage = !bFirstImage; // 이미지 교체

        Ulllast_SwitchTime = ullCurrentTime;   // 마지막 전환 시간을 넣기

    }
    // 스크롤 값을 가져옴
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // 현재 이미지 선택
    LPCWSTR currentImage = bFirstImage ? L"Fire_Monster1" : L"Fire_Monster2";

    // 이미지 핸들 가져오기
    HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(currentImage);

    // 이미지 출력 (투명 처리 포함)
    GdiTransparentBlt(
        hDC,                            // 출력 대상 DC
        m_tRect.left + iScrollX,        // 출력 위치 X
        m_tRect.top + iScrollY,         // 출력 위치 Y
        int(m_tInfo.fCX) - 10,          // 출력 이미지 너비
        int(m_tInfo.fCY),               // 출력 이미지 높이
        hMemDC,                         // 원본 이미지 DC
        bFirstImage ? 622 : 626,       // 복사할 원본 이미지 시작 X 좌표 (이미지에 따라 다름)
        bFirstImage ? 420 : 469,       // 복사할 원본 이미지 시작 Y 좌표 (이미지에 따라 다름)
        int(m_tInfo.fCX) - 10,          // 원본 이미지에서 복사할 너비
        int(m_tInfo.fCY),               // 원본 이미지에서 복사할 높이
        RGB(128, 0, 128));              // 투명 색상
}

void CMonster_1::Release()
{
}



