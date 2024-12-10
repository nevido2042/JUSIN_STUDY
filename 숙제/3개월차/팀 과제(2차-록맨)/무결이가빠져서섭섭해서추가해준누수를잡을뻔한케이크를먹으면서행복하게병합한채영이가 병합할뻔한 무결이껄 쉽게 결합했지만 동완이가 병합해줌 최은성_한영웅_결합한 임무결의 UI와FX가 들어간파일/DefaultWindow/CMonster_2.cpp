#include "pch.h"
#include "CMonster_2.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"

CMonster_2::CMonster_2()
{
}

CMonster_2::~CMonster_2()
{
}

void CMonster_2::Initialize()
{
    fGravity = 3.f;
    m_fSpeed = 3.f;
    m_tInfo = { 400.f,400,50,50 };
    m_iHp = 10;

    m_pPlayer = CObjMgr::Get_Instance()->Get_Player();

    if (m_eDir == DIR_LEFT)
        m_fSpeed *= -1;

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Monster_Search");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Monster_Ready");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Monster_Attack");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Monster_Jump");
}

int CMonster_2::Update()
{

    float Distance = CCollisionMgr::Collision_RangeChack(m_pPlayer,this);

    if (Distance > 200)
    {
        m_eMonsterState = MONSTER_SEARCH;
    }
    else
    {
        m_eMonsterState = MONSTER_READY;
    
    }
  

    // 중력대신..
    m_tInfo.fY += fGravity;
    if (780 > m_tInfo.fY && 510 < m_tInfo.fY)
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

void CMonster_2::Late_Update()
{
}

void CMonster_2::Render(HDC hDC)
{
    // 스크롤 값 가져오기
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // 상태에 따라 이미지 선택 및 원본 이미지 좌표 설정
    LPCWSTR currentImage = nullptr;
    int isrcX = 0, isrcY = 0;

    if (m_eMonsterState == MONSTER_SEARCH)
    {
        currentImage = L"Monster_Search";
        isrcX = 256;
        isrcY = 522;
    }
    else if (m_eMonsterState == MONSTER_READY)
    {
        currentImage = L"Monster_Ready";
        isrcX = 318;
        isrcY = 518;
    }

    if (currentImage)
    {
        // 현재 상태에 해당하는 이미지 가져오기
        HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(currentImage);

        GdiTransparentBlt(
            hDC,                           // 출력 대상 DC
            m_tRect.left + iScrollX,       // 출력 위치 X
            m_tRect.top + iScrollY,        // 출력 위치 Y
            int(m_tInfo.fCX),              // 출력 이미지 너비
            int(m_tInfo.fCY + 15),         // 출력 이미지 높이
            hMemDC,                        // 원본 이미지 DC
            isrcX,                          // 원본 이미지 시작 X 좌표
            isrcY,                          // 원본 이미지 시작 Y 좌표
            int(m_tInfo.fCX),              // 원본 이미지에서 복사할 너비
            int(m_tInfo.fCY + 15),         // 원본 이미지에서 복사할 높이
            RGB(128, 0, 128));             // 투명 색상
    }
}
void CMonster_2::Release()
{
}
