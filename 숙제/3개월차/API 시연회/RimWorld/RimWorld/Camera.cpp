#include "pch.h"
#include "Camera.h"
#include "ObjMgr.h"
#include "KeyMgr.h"

CCamera* CCamera::m_pInstance = nullptr;

CCamera::CCamera()
    :m_fHeight(0.f), m_fWidth(0.f), m_fX(0.f), m_fY(0.f), m_fZoom(0.f),
    m_fViewHeight(0.f), m_fViewWidth(0.f), m_fMoveX(0.f), m_fMoveY(0.f), m_bMoveLerp(false)
{
}

CCamera::~CCamera()
{
}

void CCamera::Move_To_Lerp(float _fX, float _fY)
{
    m_fMoveX = _fX;
    m_fMoveY = _fY;
    m_bMoveLerp = true;
}

void CCamera::Initialize()
{
    m_fWidth = WINCX;
    m_fHeight = WINCY;

    m_fX = m_fWidth * 0.5f;
    m_fY = m_fHeight * 0.5f;

    m_fZoom = 1.f;
}

void CCamera::Update()
{
    float fSpeed(10.f);
    if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
    {
        m_bMoveLerp = false;
        m_fX -= fSpeed;
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
    {
        m_bMoveLerp = false;
        m_fX += fSpeed;
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
    {
        m_bMoveLerp = false;
        m_fY -= fSpeed;
    }
    if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
    {
        m_bMoveLerp = false;
        m_fY += fSpeed;
    }

    // ȭ�� ũ��
    //int screenWidth = WINCX;
    //int screenHeight = WINCY;

    if (CKeyMgr::Get_Instance()->Key_Pressing(VK_ADD)) // ����
    {
        m_fZoom += 0.01f;
        if (m_fZoom > 3.0f) m_fZoom = 3.0f; // �ִ� �� ����
    }

    if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SUBTRACT)) // �ܾƿ�
    {
        m_fZoom -= 0.01f;
        if (m_fZoom < 0.2f) m_fZoom = 0.2f; // �ּ� �� ����
    }

    if (CKeyMgr::Get_Instance()->Key_Pressing(VK_NUMPAD0)) // �� ���� �⺻
    {
        m_fZoom = 1.f;
    }
}

void CCamera::Late_Update()
{
    if (m_bMoveLerp)
    {
        // ��ǥ ��ġ ���
        float fTargetX = m_fMoveX;// -m_fMoveX + WINCX * 0.5f;
        float fTargetY = m_fMoveY;// -m_fMoveY + WINCY * 0.5f;

        // ���� ��ġ ��������
        float fCurrentX = Get_X();
        float fCurrentY = Get_Y();

        // ���� ���� ���� (0.0f ~ 1.0f)
        float fLerpFactor = 0.1f; // 0.1f = ������ �̵�, 1.0f = ��� �̵�

        // LERP ���
        float fNewX = fCurrentX + (fTargetX - fCurrentX) * fLerpFactor;
        float fNewY = fCurrentY + (fTargetY - fCurrentY) * fLerpFactor;

        // ���ο� ��ġ ����
        //CScrollMgr::Get_Instance()->Set_ScrollX(fNewX);
        //CScrollMgr::Get_Instance()->Set_ScrollY(fNewY);
        Set_Pos(fNewX, fNewY);

        // �Ÿ� ��� (��Ŭ���� �Ÿ�)
        float fDeltaX = fTargetX - fCurrentX;
        float fDeltaY = fTargetY - fCurrentY;
        float fDistance = sqrt(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

        // ��ǥ ��ġ�� ��������� ����
        if (fDistance < 1.0f) { // 1.0f�� �Ӱ谪(��� ����)���� ���� ����
            m_bMoveLerp = false;
        }
    }
}

bool CCamera::IsInCameraView(float _fX, float _fY, float _fWidth, float _fHeight)
{
    float fLeft = m_fX - (m_fWidth * 0.5f / m_fZoom + TILECX * 0.5f);
    float fRight = m_fX + (m_fWidth * 0.5f / m_fZoom + TILECX * 0.5f);
    float fTop = m_fY - (m_fHeight * 0.5f / m_fZoom + TILECY * 0.5f);
    float fBottom = m_fY + (m_fHeight * 0.5f / m_fZoom + TILECY * 0.5f);

    m_fViewWidth = abs(fLeft - fRight);
    m_fViewHeight = abs(fTop - fBottom);

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

    swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"Camera W/H: %.2f, %.2f", (double)m_fWidth, (double)m_fHeight);
    // ���ڿ� ��� (�����ڵ�)
    TextOutW(hDC, 0, 60, buffer, lstrlenW(buffer));

    swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"Camera X/Y: %.2f, %.2f", (double)m_fX, (double)m_fY);
    // ���ڿ� ��� (�����ڵ�)
    TextOutW(hDC, 0, 80, buffer, lstrlenW(buffer));

    swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"Camera Zoom: %.2f", (double)m_fZoom);
    // ���ڿ� ��� (�����ڵ�)
    TextOutW(hDC, 0, 100, buffer, lstrlenW(buffer));

    swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"View W/H: %.2f, %.2f", (double)m_fViewWidth, (double)m_fViewHeight);
    // ���ڿ� ��� (�����ڵ�)
    TextOutW(hDC, 0, 120, buffer, lstrlenW(buffer));
}
