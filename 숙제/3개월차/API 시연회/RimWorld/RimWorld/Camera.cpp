#include "pch.h"
#include "Camera.h"
#include "ObjMgr.h"
#include "KeyMgr.h"

CCamera* CCamera::m_pInstance = nullptr;

CCamera::CCamera()
	:m_fHeight(0.f), m_fWidth(0.f), m_fX(0.f), m_fY(0.f), m_fZoom(0.f)
{
}

CCamera::~CCamera()
{
}

void CCamera::Initialize()
{
    m_fWidth = WINCX;
    m_fHeight = WINCY;

    m_fX = m_fWidth * 0.5f;
    m_fY = m_fWidth * 0.5f;

    m_fZoom = 1.f;
}

void CCamera::Update()
{
    float fSpeed(10.f);
    if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
    {
        m_fX -= fSpeed;
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
    {
        m_fX += fSpeed;
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
    {
        m_fY -= fSpeed;
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
    {
        m_fY += fSpeed;
    }

    // ȭ�� ũ��
    int screenWidth = WINCX;
    int screenHeight = WINCY;

    // ī�޶� �� ��/�ƿ�
    if (CKeyMgr::Get_Instance()->Key_Pressing(VK_ADD))
    {
        m_fZoom += 0.1f;
        //m_fWidth = screenWidth / m_fZoom;
        //m_fHeight = screenHeight / m_fZoom;

        // ī�޶� ��ġ ���� (ȭ�� �߾��� ��������)
        //m_fX = (m_fX + screenWidth / 2) / m_fZoom - screenWidth / 2;
        //m_fY = (m_fY + screenHeight / 2) / m_fZoom - screenHeight / 2;
    }

    if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SUBTRACT))
    {
        m_fZoom -= 0.1f;
        //m_fWidth = screenWidth / m_fZoom;
        //m_fHeight = screenHeight / m_fZoom;

        // ī�޶� ��ġ ���� (ȭ�� �߾��� ��������)
        //m_fX = (m_fX + screenWidth / 2) / m_fZoom - screenWidth / 2;
        //m_fY = (m_fY + screenHeight / 2) / m_fZoom - screenHeight / 2;
    }


    if (m_fZoom < 0.1f) m_fZoom = 0.1f; // �ּ� �� ����
    if (m_fZoom > 5.0f) m_fZoom = 5.0f; // �ִ� �� ����
}

bool CCamera::IsInCameraView(float _fX, float _fY, float _fWidth, float _fHeight)
{
    float fLeft = m_fX - m_fWidth * 0.5f;
    float fRight = m_fX + m_fWidth * 0.5f;
    float fTop = m_fY - m_fHeight * 0.5f;
    float fBottom = m_fY + m_fHeight * 0.5f;

    // ��ü�� ī�޶� ����Ʈ�� ��ġ���� Ȯ��
    return !(_fX + _fWidth < fLeft || _fX > fRight || _fY + _fHeight < fTop || _fY > fBottom);
}

void CCamera::Render(HDC hDC)
{
    // ���� ���� �����ڵ� ���ڿ��� ��ȯ
    //wchar_t buffer[50];
    //wsprintf(buffer, L"m_iCurFrame: %d", m_iCurFrame);
    //// ���ڿ� ��� (�����ڵ�)
    //TextOutW(hDC, 0, 0, buffer, lstrlenW(buffer));

    //���콺 Ŭ�� ���� �� Ÿ������ ����
    POINT	ptMouse{};
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    wchar_t buffer[256];
    swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"Mouse Screen: %.2f, %.2f", (double)ptMouse.x, (double)ptMouse.y);
    // ���ڿ� ��� (�����ڵ�)
    TextOutW(hDC, 0, 20, buffer, lstrlenW(buffer));

    POINT tWorldPoint = ScreenToWorld((int)ptMouse.x, (int)ptMouse.y);

    swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"Mouse World: %.2f, %.2f", (double)tWorldPoint.x, (double)tWorldPoint.y);
    // ���ڿ� ��� (�����ڵ�)
    TextOutW(hDC, 0, 40, buffer, lstrlenW(buffer));
}
