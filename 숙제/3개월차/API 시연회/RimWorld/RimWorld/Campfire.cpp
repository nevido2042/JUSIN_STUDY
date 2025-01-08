#include "pch.h"
#include "Campfire.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CCampfire::CCampfire()
{
}

CCampfire::~CCampfire()
{
	Release();
}

void CCampfire::Initialize()
{
	Set_Size(64.f, 64.f);

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	//m_tFrame.ullSpeed = 200;
	m_tFrame.ullTime = 10;

	m_eRenderID = RENDER_GAMEOBJECT;
}

void CCampfire::Late_Update()
{
	__super::Move_Frame();
}

void CCampfire::Render(HDC hDC)
{

    float fZoom = CCamera::Get_Instance()->Get_Zoom();

    HDC hFireDC = CBmpMgr::Get_Instance()->Find_Image(L"FireAnimated");
    HDC hCampfireDC = CBmpMgr::Get_Instance()->Find_Image(L"Campfire");

    // ���� ��ǥ�� ��ũ�� ��ǥ�� ��ȯ (�� �ݿ�)
    POINT tScreenPoint = CCamera::Get_Instance()->WorldToScreen(m_tInfo.fX, m_tInfo.fY);

    // ù ��° �̹���: Campfire
    GdiTransparentBlt(hDC,
        (int)(tScreenPoint.x - (m_tInfo.fCX / 2) * fZoom),  // �߽��� ���� ��ǥ ���� (����)
        (int)(tScreenPoint.y - (m_tInfo.fCY / 2) * fZoom),  // �߽��� ���� ��ǥ ���� (����)
        (int)(m_tInfo.fCX * fZoom),                        // ������� �̹����� ���� (�� �ݿ�)
        (int)(m_tInfo.fCY * fZoom),                        // ������� �̹����� ���� (�� �ݿ�)
        hCampfireDC,
        0,
        0,
        (int)m_tInfo.fCX,                                  // ���� ���� ũ��
        (int)m_tInfo.fCY,                                  // ���� ���� ũ��
        RGB_WHITE);

    // �� ��° �̹���: FireAnimated
    GdiTransparentBlt(hDC,
        (int)(tScreenPoint.x - (m_tInfo.fCX * 0.45f) * fZoom),  // ��ǥ ���� (����) + �� �ݿ�
        (int)(tScreenPoint.y - (m_tInfo.fCY * 0.55f) * fZoom),  // ��ǥ ���� (����) + �� �ݿ�
        (int)((m_tInfo.fCX * 0.9f) * fZoom),                   // ������� �̹����� ���� (�� �ݿ�)
        (int)((m_tInfo.fCY * 0.9f) * fZoom),                   // ������� �̹����� ���� (�� �ݿ�)
        hFireDC,
        (int)(m_tInfo.fCX * m_tFrame.iFrameStart),             // ��Ʈ�� ��� ���� ��ǥ (X)
        (int)(m_tInfo.fCY * m_tFrame.iMotion),                 // ��Ʈ�� ��� ���� ��ǥ (Y)
        (int)m_tInfo.fCX,                                      // ���� ���� ũ��
        (int)m_tInfo.fCY,                                      // ���� ���� ũ��
        RGB_PURPLE);

}

void CCampfire::Release()
{
}
