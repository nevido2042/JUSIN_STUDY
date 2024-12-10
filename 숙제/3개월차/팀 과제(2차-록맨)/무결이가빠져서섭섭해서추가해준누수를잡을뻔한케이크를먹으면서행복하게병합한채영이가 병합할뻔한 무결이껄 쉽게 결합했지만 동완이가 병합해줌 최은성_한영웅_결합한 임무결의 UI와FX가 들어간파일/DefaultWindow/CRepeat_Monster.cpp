#include "pch.h"
#include "CRepeat_Monster.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
CRepeat_Monster::CRepeat_Monster() : m_eState(END), m_fDinstance(0), m_iDirection(1), m_fMinX(0), m_fMaxX(0)
{
    ZeroMemory(&m_tPartol, sizeof(IMAGE));
    ZeroMemory(&m_tAttack, sizeof(IMAGE));
    ZeroMemory(&m_tNowState, sizeof(IMAGE));
}
CRepeat_Monster::~CRepeat_Monster()
{
}
void CRepeat_Monster::Initialize()
{
    // 임시
    m_tInfo.fX = 200.f;
    m_tInfo.fY = 400.f;
    m_fMinX = 100.f;
    m_fMaxX = 300.f;

    m_iHp = 10;
    m_tInfo.fCX = 33.f;
    m_tInfo.fCY = 16.f;
    m_fSpeed = 2.f;
    m_fDinstance = 100.f;

    m_pTarget = CObjMgr::Get_Instance()->Get_Player();
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Enemy");

    m_tPartol = { 129,363 };
    m_tAttack = { 173,363 };
}

int CRepeat_Monster::Update()
{
    //  무적 몬스터임

    if (m_fDinstance > CCollisionMgr::Collision_RangeChack(m_pTarget,this))
    {
        m_eState = ATTACK;
        m_fSpeed = 5.f;

        Patrol();
           
    }
    else
    {
        m_eState = PATROL;
        m_fSpeed = 2.f;
        
        Patrol();
    }
    m_tInfo.fX += m_fSpeed * m_iDirection;

    Jumping(m_tInfo.fCY *0.5);

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CRepeat_Monster::Late_Update()
{
    switch (m_eState)
    {
    case PATROL:
        m_tNowState = m_tPartol;
        break;
    case ATTACK:
        m_tNowState = m_tAttack;
        break;
    default:
        break;
    }
   
}

void CRepeat_Monster::Render(HDC hDC)
{
    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Enemy");

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    GdiTransparentBlt(hDC,						// 복사 받을 DC
        m_tInfo.fX + iScrollX,
        m_tInfo.fY + iScrollY,					// 복사 받을 위치 좌표 X, Y			
        m_tInfo.fCX, m_tInfo.fCY,				// 복사 받을 이미지의 가로, 세로
        hMemDC,									// 복사할 이미지 DC	
        m_tNowState._iX, m_tNowState._iY,								// 비트맵 출력 시작 좌표(Left, top)
        m_tInfo.fCX, m_tInfo.fCY,									// 복사할 이미지의 가로, 세로
        RGB(128, 0, 128));						// 출력 효과 설정(그대로 출력)
}

void CRepeat_Monster::Release()
{
}

void CRepeat_Monster::Patrol()
{
    m_tInfo.fX += m_fSpeed * m_iDirection;

    // 경계 충돌 체크
    if (m_tInfo.fX >= m_fMaxX) // 왼쪽 이동
    {
        m_tInfo.fX = m_fMaxX;
        m_iDirection = -1;    
    }
    else if (m_tInfo.fX <= m_fMinX) // 오른쪽 이동
    {
        m_tInfo.fX = m_fMinX; 
        m_iDirection = 1;     
    }

}
