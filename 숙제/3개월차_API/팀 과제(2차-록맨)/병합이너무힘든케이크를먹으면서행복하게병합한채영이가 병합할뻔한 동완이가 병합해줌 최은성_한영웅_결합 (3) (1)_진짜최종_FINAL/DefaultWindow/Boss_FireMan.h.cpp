#include "pch.h"
#include "Boss_FireMan.h"
#include "CBullet.h"
#include "CScrollMgr.h"
#include "CLineMgr.h"
#include "BlockMgr.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CFire_Storm.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"

#include "CUIMgr.h"
#include "CBossHpBar.h"
#include "CEffect.h"
CBoss_FireMan::CBoss_FireMan()
{
    ZeroMemory(&m_p_Boss_Bullet, sizeof(m_p_Boss_Bullet));
    ZeroMemory(&HP_INFO, sizeof(HP_INFO));
    m_Boss_pPlayer = NULL;
    m_ullLast_Fire = 0;

    m_iHp = 80;

    Hp_Count = 0;

    m_bJump = false;
    m_fJumpPower = 0.f;
    m_fTime = 0.f;
    m_ullLast_Fire = 0;
    m_ullLast_Fire = GetTickCount64();
    ////////////////////////
    bool m_bMovingLeft;
    int m_fJumpCooldown;

    MAX_Hp = 0;
    Hp_Count = 0;// 보스에 충돌할때마다 체크
    m_Boss_Ground = true;

    m_pPlayer = nullptr;
    bool m_Boss_Ground;

}

CBoss_FireMan::~CBoss_FireMan()
{
}

void CBoss_FireMan::Initialize()
{

    m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_fire_all.bmp", L"Fire_Man");

    m_tInfo = { 3800, 2200, 42.f, 48.f };
    HP_INFO = { 200,200,200.f,200.f };
    m_fSpeed = 3.f;
    MAX_Hp = 10;

    m_eBossType = BT_FIREMAN;
    CUIMgr::Get_Instance()->Add_UI(UI_HP_BOSS, CAbstractFactory<CBossHpBar>::CreateUI(this));

}


//CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CFire_Storm>::Create(m_tInfo.fX, m_tInfo.fY, DIR_RIGHT));


int CBoss_FireMan::Update()
{

    if (m_bDead)
        return OBJ_DEAD;



    if (m_bDead || m_iHp <= 0)
    {
        CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CEffect>::Create(m_tInfo.fX - 10.f, m_tInfo.fY - 5.f));
        return OBJ_DEAD;
    }

    Jumping();

    float Distance = CCollisionMgr::Collision_RangeChack(m_pPlayer, this);

    if (Distance < 300) // 300보다 가까우면
    {
        // 보스가 플레이어를 피해 이동
        if (m_tInfo.fX < m_pPlayer->Get_Info().fX)
            m_tInfo.fX -= m_fSpeed; // 왼쪽으로 이동
        else
            m_tInfo.fX += m_fSpeed; // 오른쪽으로 이동
    }
    else // Distance >= 300 (거리를 유지하고 있을 때)
    {
        // x축 기준 거리 계산
        float xDistance = abs(m_tInfo.fX - m_pPlayer->Get_Info().fX);

        // 발사 조건: 400 거리 유지 중, x축 1000 이내, 0.8초마다 발사
        if (xDistance <= 1000 && m_ullLast_Fire + 800 < GetTickCount64())
        {
            m_ullLast_Fire = GetTickCount64(); // 마지막 발사 시간 갱신

            if (m_tInfo.fX > m_pPlayer->Get_Info().fX) // 플레이어의 왼쪽에 있을 때
            {
                CObjMgr::Get_Instance()->Add_Object(
                    OBJ_BOSSBULLET,
                    CAbstractFactory<CFire_Storm>::Create(m_tInfo.fX, m_tInfo.fY, DIR_LEFT)
                );
            }
            else // 플레이어의 오른쪽에 있을 때
            {
                CObjMgr::Get_Instance()->Add_Object(
                    OBJ_BOSSBULLET,
                    CAbstractFactory<CFire_Storm>::Create(m_tInfo.fX, m_tInfo.fY, DIR_RIGHT)
                );
            }
        }
    }

    Update_Rect();
    return OBJ_NOEVENT;
}





void CBoss_FireMan::Late_Update()
{
}


void CBoss_FireMan::Render(HDC hDC)
{
    // 스크롤 위치 계산
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC hMeDC = CBmpMgr::Get_Instance()->Find_Image(L"Fire_Man");

    // 보스 이미지 렌더링
    GdiTransparentBlt(
        hDC,
        m_tRect.left + iScrollX,              // X 좌표
        m_tRect.top + iScrollY - 29,          // Y 좌표를 수정 (보스가 바닥에 맞게 렌더링되도록 오프셋 추가)
        68,                                   // 이미지 가로 크기
        77,                                   // 이미지 세로 크기
        hMeDC,
        51,
        15,
        68,
        77,
        RGB(128, 0, 128));



    // 체력 바 그리기
    Rectangle(hDC, 90, 80, 110, 200); // 전체 체력 바 (최대 체력)

    // 체력 바 색상 설정
    HBRUSH MyBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);

    // 현재 체력에 따른 체력 바 길이 계산&
    Rectangle(hDC, 90, 80, 110, (200 - Hp_Count));

    // 브러시 해제
    SelectObject(hDC, OldBrush);
    DeleteObject(MyBrush);
}

CObj* CBoss_FireMan::Create_Bullet(int _type)
{
    return nullptr;
}









