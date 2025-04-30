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

    // �� ���
    m_tPoint[0].x = LONG((m_tInfo.fCX / 2.f) + (fDiagonal * cosf((m_fAngle + 135.f) * PI / 180.f)));
    m_tPoint[0].y = LONG((m_tInfo.fCY / 2.f) - (fDiagonal * sinf((m_fAngle + 135.f) * PI / 180.f)));

    // �� ���
    m_tPoint[1].x = LONG((m_tInfo.fCX / 2.f) + (fDiagonal * cosf((m_fAngle + 45.f) * PI / 180.f)));
    m_tPoint[1].y = LONG((m_tInfo.fCY / 2.f) - (fDiagonal * sinf((m_fAngle + 45.f) * PI / 180.f)));

    // �� �ϴ�
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

    // 1. ȸ���� ���� �׸���.

    PlgBlt(hPlgDC,      // ȸ�� ��Ų �̹����� ��� �׸� ���ΰ�
        m_tPoint,       // �׸��� �׸� �簢���� ��ǥ �� POINT ������ 3��(�»��, ����, ���ϴ� ����)
        hMemDC,         // ��� �ִ� �̹����� ȸ������ �׸� ���ΰ�
        0, 0,           // ȸ�� ���� �׸� �̹����� ���� ��ǥ
        (int)m_tInfo.fCX,   // ȸ�� ���� �׸� �̹����� ����, ���� ������
        (int)m_tInfo.fCY,
        NULL, NULL, NULL);      // ���� ����ŷ �ؽ�ó ���� NULL


    // 2. ���ʿ��� �ȼ� ������ �����Ͽ� �׸���.
    GdiTransparentBlt(hDC,			// ���� ���� DC
        m_tRect.left + iScrollX,	// ���� ���� ��ġ ��ǥ X, Y	
        m_tRect.top + iScrollY,
        (int)m_tInfo.fCX,			// ���� ���� �̹����� ����, ����
        (int)m_tInfo.fCY,
        hPlgDC,						// ������ �̹��� DC	
        0,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
        0,
        (int)m_tInfo.fCX,			// ������ �̹����� ����, ����
        (int)m_tInfo.fCY,
        RGB(255, 255, 255));		// ������ ����

    // 3. ���� �������� ���� �ι��� �̹����� ���� �׸���.

    BitBlt(hPlgDC, 
        0, 0,
        (int)m_tInfo.fCX,
        (int)m_tInfo.fCY,
        hResetDC, 0, 0, SRCCOPY);
}

void CMonster::Release()
{
}
