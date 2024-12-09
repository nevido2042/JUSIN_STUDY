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
  

    // �߷´��..
    m_tInfo.fY += fGravity;
    if (780 > m_tInfo.fY && 510 < m_tInfo.fY)
        fGravity = 0.f;

    m_tInfo.fX -= m_fSpeed;

    if (m_tInfo.fX > 800 || m_tInfo.fX < 100)
        m_fSpeed *= -1.f;

    // �׾��� ��� ó��
    if (m_bDead)
        return OBJ_DEAD;

    // �簢�� ����
    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CMonster_2::Late_Update()
{
}

void CMonster_2::Render(HDC hDC)
{
    // ��ũ�� �� ��������
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // ���¿� ���� �̹��� ���� �� ���� �̹��� ��ǥ ����
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
        // ���� ���¿� �ش��ϴ� �̹��� ��������
        HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(currentImage);

        GdiTransparentBlt(
            hDC,                           // ��� ��� DC
            m_tRect.left + iScrollX,       // ��� ��ġ X
            m_tRect.top + iScrollY,        // ��� ��ġ Y
            int(m_tInfo.fCX),              // ��� �̹��� �ʺ�
            int(m_tInfo.fCY + 15),         // ��� �̹��� ����
            hMemDC,                        // ���� �̹��� DC
            isrcX,                          // ���� �̹��� ���� X ��ǥ
            isrcY,                          // ���� �̹��� ���� Y ��ǥ
            int(m_tInfo.fCX),              // ���� �̹������� ������ �ʺ�
            int(m_tInfo.fCY + 15),         // ���� �̹������� ������ ����
            RGB(128, 0, 128));             // ���� ����
    }
}
void CMonster_2::Release()
{
}
