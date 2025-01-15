#include "pch.h"
#include "CPlayer.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo.vPos = { 400.f, 300.f, 0.f };
	m_fSpeed = 10.f;
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

}

void CPlayer::Update()
{
	m_tInfo.vDir = ::Get_Mouse() - m_tInfo.vPos;

	//float	fLength = sqrtf(m_tInfo.vDir.x * m_tInfo.vDir.x + m_tInfo.vDir.y * m_tInfo.vDir.y);
	//
	//m_tInfo.vDir.x /= fLength;
	//m_tInfo.vDir.y /= fLength;
	//m_tInfo.vDir.z = 0.f;
	//
	//float	fLength2 = sqrtf(m_tInfo.vLook.x * m_tInfo.vLook.x + m_tInfo.vLook.y * m_tInfo.vLook.y);
	//
	//m_tInfo.vLook.x /= fLength2;
	//m_tInfo.vLook.y /= fLength2;
	//m_tInfo.vLook.z = 0.f;
	//
	//float	fDot = m_tInfo.vDir.x * m_tInfo.vLook.x + m_tInfo.vDir.y * m_tInfo.vLook.y;
	//
	//float	fAngle = acosf(fDot);
	//
	//if (m_tInfo.vPos.y < Get_Mouse().y)
	//	fAngle = 2.f * D3DX_PI - fAngle;
	//
	//m_tInfo.vPos.x += cosf(fAngle) * m_fSpeed;
	//m_tInfo.vPos.y -= sinf(fAngle) * m_fSpeed;

	// A • B : 스칼라 곱
	// A X B : 벡터 곱

	// D3DXVec3Normalize(단위 벡터로 만든 결과 값을 저장할 벡터의 주소, 정규화를 수행할 벡터의 주소)
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	D3DXVec3Normalize(&m_tInfo.vLook, &m_tInfo.vLook);

	// D3DXVec3Length(크기를 구하고 싶은 벡터의 주소)
	// float fLength = D3DXVec3Length(&m_tInfo.vDir);

	// float fDot = D3DXVec3Dot(방향 벡터1의 주소, 방향 벡터2의 주소)
	float	fDot = D3DXVec3Dot(&m_tInfo.vDir, &m_tInfo.vLook);

	// 외적의 경우 2, 3인자의 순서가 중요
	// D3DXVECTOR3*	D3DXVec3Cross(결과 값을 저장할 벡터의 주소, 방향 벡터 1의 주소, 방향 벡터 2의 주소)

	// D3DXVECTOR3		vAxis;
	// D3DXVec3Cross(&vAxis, &m_tInfo.vDir, &m_tInfo.vLook);

	float	fAngle = acosf(fDot);

	if (m_tInfo.vPos.y < Get_Mouse().y)
		fAngle = 2.f * D3DX_PI - fAngle;

	m_tInfo.vPos.x += cosf(fAngle) * m_fSpeed;
	m_tInfo.vPos.y -= sinf(fAngle) * m_fSpeed;
}

void CPlayer::Render(HDC hDC)
{
	Rectangle(hDC, 
		int(m_tInfo.vPos.x - 50.f), 
		int(m_tInfo.vPos.y - 50.f), 
		int(m_tInfo.vPos.x + 50.f),
		int(m_tInfo.vPos.y + 50.f));
}

void CPlayer::Release()
{
}
