#include "pch.h"
#include "Rim.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CRim::CRim()
{
}

CRim::~CRim()
{
    Release();
}

void CRim::Initialize()
{
    m_tInfo.fCX = 128.f;
    m_tInfo.fCY = 152.f;
}

int CRim::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

int CRim::Late_Update()
{
    return OBJ_NOEVENT;
}

void CRim::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // �� ����, ��, �Ӹ� �� �׽�Ʈ
    HDC hTestDC;
    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_south");

    // GdiTransparentBlt ����Ͽ� ���� ó�� �� ũ�� ���
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX, m_tRect.top + iScrollY, 128, 128,  // ��� ���� (���� ũ��)
        hTestDC, 0, 0, 128, 128,  // ���� �̹��� ����
        RGB_PURPLE);  // ���� ����

    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_south");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX, m_tRect.top + iScrollY - HEAD_OFFSET, 128, 128,
        hTestDC, 0, 0, 128, 128,
        RGB_PURPLE);

    // ������ī �Ӹ� �̹���
    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Afro_south");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX, m_tRect.top + iScrollY - HEAD_OFFSET, 128, 128,
        hTestDC, 0, 0, 128, 128,
        RGB_PURPLE);
}

void CRim::Release()
{
}
