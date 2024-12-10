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

	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Fire_Monster1");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/enemy_all.bmp", L"Fire_Monster2");
}

int CMonster_1::Update()
{
   // �߷´��..
    m_tInfo.fY += fGravity;
    if (780 > m_tInfo.fY && 522 < m_tInfo.fY)
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

void CMonster_1::Late_Update()
{
}

void CMonster_1::Render(HDC hDC)
{
    static ULONGLONG Ulllast_SwitchTime = 0; // ���������� �̹����� ����� �ð�

    static bool bFirstImage = true;    // ���� ǥ�� ���� �̹��� 

    ULONGLONG ullCurrentTime = GetTickCount64();
    if (ullCurrentTime - Ulllast_SwitchTime > 400) //  0.4��
    {
        bFirstImage = !bFirstImage; // �̹��� ��ü

        Ulllast_SwitchTime = ullCurrentTime;   // ������ ��ȯ �ð��� �ֱ�

    }
    // ��ũ�� ���� ������
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // ���� �̹��� ����
    LPCWSTR currentImage = bFirstImage ? L"Fire_Monster1" : L"Fire_Monster2";

    // �̹��� �ڵ� ��������
    HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(currentImage);

    // �̹��� ��� (���� ó�� ����)
    GdiTransparentBlt(
        hDC,                            // ��� ��� DC
        m_tRect.left + iScrollX,        // ��� ��ġ X
        m_tRect.top + iScrollY,         // ��� ��ġ Y
        int(m_tInfo.fCX) - 10,          // ��� �̹��� �ʺ�
        int(m_tInfo.fCY),               // ��� �̹��� ����
        hMemDC,                         // ���� �̹��� DC
        bFirstImage ? 622 : 626,       // ������ ���� �̹��� ���� X ��ǥ (�̹����� ���� �ٸ�)
        bFirstImage ? 420 : 469,       // ������ ���� �̹��� ���� Y ��ǥ (�̹����� ���� �ٸ�)
        int(m_tInfo.fCX) - 10,          // ���� �̹������� ������ �ʺ�
        int(m_tInfo.fCY),               // ���� �̹������� ������ ����
        RGB(128, 0, 128));              // ���� ����
}

void CMonster_1::Release()
{
}



