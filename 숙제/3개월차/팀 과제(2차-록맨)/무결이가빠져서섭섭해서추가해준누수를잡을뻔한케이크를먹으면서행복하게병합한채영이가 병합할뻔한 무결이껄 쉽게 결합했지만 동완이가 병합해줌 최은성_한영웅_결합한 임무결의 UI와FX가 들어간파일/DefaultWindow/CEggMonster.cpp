#include "pch.h"
#include "CEggMonster.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include "CBullet_Monter_Elec.h"
#include "CAbstractFactory.h"

CEggMonster::CEggMonster(): m_eState(END), m_fDinstance(0), m_iDirection(1), m_fMinY(0), m_fMaxY(0), m_iCount(0), m_pBulletList(nullptr)
{
    ZeroMemory(&m_tMove, sizeof(IMAGEINFO));
    ZeroMemory(&m_tAttack1, sizeof(IMAGEINFO));
    ZeroMemory(&m_tAttack2, sizeof(IMAGEINFO));
    ZeroMemory(&m_tNowState, sizeof(IMAGEINFO));
}

CEggMonster::~CEggMonster()
{
}

void CEggMonster::Initialize()
{
    //임시
    m_tInfo = { 400, 600, 34, 40 };
    m_fMinY = 200;
    m_fMaxY = 500;

    m_iHp = 10;
    m_pTarget = CObjMgr::Get_Instance()->Get_Player();
    m_pBulletList = CObjMgr::Get_Instance()->Get_List(OBJ_BOSSBULLET);

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Enemy");
    m_fSpeed = 1.f;

    // 좌표xy, / 이미지 크기 cx,cy
    m_tMove = { 763, 209, 34, 40 };
    m_tAttack1 = { 801, 203, 34, 50 };
    m_tAttack2 = { 839, 185, 34, 86 };

}

int CEggMonster::Update()
{
    if (m_bDead ||m_iHp <=0)
        return OBJ_DEAD;

    Patrol();
    if (GetDistance() < 200.f)
        m_eState = ATTACK;
    else
        m_eState = IDLE;
    
    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CEggMonster::Late_Update()
{
    switch (m_eState)
    {
    case IDLE:
        m_tNowState = m_tMove;
        break;
    case ATTACK:
        if ((m_tNowState == m_tMove))
        {
            if (++m_iCount >= 40)
            {
                m_tNowState = m_tAttack1;
                m_iCount = 0;
            }
        }
        else if (m_tNowState == m_tAttack1)
        {
            if (++m_iCount >= 20)
            {
                m_tNowState = m_tAttack2;
                m_pBulletList->push_back(Create_Bullet(m_tInfo.fY - 35.f));
                m_pBulletList->push_back(Create_Bullet(m_tInfo.fY + 35.f));
                m_iCount = 0;
            }
        }
        else if ((m_tNowState == m_tAttack2))
        {
            if (++m_iCount >= 20)
            {
                m_tNowState = m_tMove;
                m_iCount = 0;
            }
        }
        break;
    default:
        break;
    }
}

void CEggMonster::Render(HDC hDC)
{
    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Enemy");

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    GdiTransparentBlt(hDC,						// 복사 받을 DC
        m_tInfo.fX + iScrollX,
        m_tInfo.fY + iScrollY,					// 복사 받을 위치 좌표 X, Y			
        m_tNowState._iCX, m_tNowState._iCY,		// 복사 받을 이미지의 가로, 세로
        hMemDC,									// 복사할 이미지 DC	
        m_tNowState._iX, m_tNowState._iY,		// 비트맵 출력 시작 좌표(Left, top)
        m_tNowState._iCX, m_tNowState._iCY,		// 복사할 이미지의 가로, 세로
        RGB(128, 0, 128));						// 출력 효과 설정(그대로 출력)
}

void CEggMonster::Release()
{
}

float CEggMonster::GetDistance()
{
    return fabsf(m_pTarget->Get_Info().fY - m_tInfo.fY);
}

void CEggMonster::Patrol()
{
    m_tInfo.fY -= m_fSpeed * m_iDirection;
    if (m_tInfo.fY <= m_fMinY)
    {
        m_tInfo.fY = m_fMinY;
        m_iDirection = -1.f;
    }
    if (m_tInfo.fY >= m_fMaxY)
    {
        m_tInfo.fY = m_fMaxY;
        m_iDirection = 1.f;
    }
}

CObj* CEggMonster::Create_Bullet(float _y)
{
    if (m_pTarget->Get_Info().fX > m_tInfo.fX)
    {
        m_fAngle = 0.f;
    }
    else if (m_pTarget->Get_Info().fX <= m_tInfo.fX)
    {
        m_fAngle = 180.f;
    }

    return CAbstractFactory<CBullet_Monter_Elec>::Create(m_tInfo.fX, _y, m_fAngle);
}