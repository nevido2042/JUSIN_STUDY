#include "pch.h"
#include "Camera.h"
#include "ObjMgr.h"
#include "KeyMgr.h"

CCamera* CCamera::m_pInstance = nullptr;

CCamera::CCamera()
	:m_fHeigth(0.f), m_fWidth(0.f), m_fX(0.f), m_fY(0.f), m_fZoom(0.f)
{
}

CCamera::~CCamera()
{
}

void CCamera::Initialize()
{
	m_fWidth = WINCX;
	m_fHeigth = WINCY;

    m_fX = WINCX * 0.5f;
    m_fY = WINCY * 0.5f;

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

    if (CKeyMgr::Get_Instance()->Key_Down(VK_ADD))
    {
        m_fZoom += 0.1f;
    }

    if (CKeyMgr::Get_Instance()->Key_Down(VK_SUBTRACT))
    {
        m_fZoom -= 0.1f;
    }

    if (m_fZoom < 0.1f) m_fZoom = 0.1f; // 최소 줌 제한
    if (m_fZoom > 5.0f) m_fZoom = 5.0f; // 최대 줌 제한
}

bool CCamera::IsInCameraView(float _fX, float _fY, float _fWidth, float _fHeight)
{
    float fLeft = m_fX - m_fWidth * 0.5f;
    float fRight = m_fX + m_fWidth * 0.5f;
    float fTop = m_fY - m_fHeigth * 0.5f;
    float fBottom = m_fY + m_fHeigth * 0.5f;

    // 객체가 카메라 뷰포트와 겹치는지 확인
    return !(_fX + _fWidth < fLeft || _fX > fRight || _fY + _fHeight < fTop || _fY > fBottom);
}

void CCamera::Render(HDC hDC)
{

}
