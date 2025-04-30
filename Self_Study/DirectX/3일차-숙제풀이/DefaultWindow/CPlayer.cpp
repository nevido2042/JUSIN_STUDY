#include "pch.h"
#include "CPlayer.h"

CPlayer::CPlayer() : m_fAngle(0.f)
{
	ZeroMemory(m_vPoint, sizeof(m_vPoint));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo.vPos = { 400.f, 300.f, 0.f };
	m_fSpeed = 3.f;
	m_tInfo.vLook = { 0.f, -1.f, 0.f };

	m_vPoint[0] = { m_tInfo.vPos.x - 50.f, m_tInfo.vPos.y - 50.f, 0.f };
	m_vPoint[1] = { m_tInfo.vPos.x + 50.f, m_tInfo.vPos.y - 50.f, 0.f };
	m_vPoint[2] = { m_tInfo.vPos.x + 50.f, m_tInfo.vPos.y + 50.f, 0.f };
	m_vPoint[3] = { m_tInfo.vPos.x - 50.f, m_tInfo.vPos.y + 50.f, 0.f };

	for (int i = 0; i < 4; ++i)
		m_vOriginPoint[i] = m_vPoint[i];

	m_vGunPoint = { m_tInfo.vPos.x, m_tInfo.vPos.y - 100.f , 0.f };

	m_vOriginGunPoint = m_vGunPoint;
}

void CPlayer::Update()
{
#pragma region 벡터만 이용한 과제 풀이
	// for (int i = 0; i < 4; ++i)
	// {
	// 	D3DXVECTOR3 vTemp = m_vOriginPoint[i];
	// 
	// 	vTemp -= { 400.f, 300.f, 0.f };
	// 
	// 	// 회전 변환
	// 	m_vPoint[i].x = vTemp.x * cosf(m_fAngle) - vTemp.y * sinf(m_fAngle);
	// 	m_vPoint[i].y = vTemp.x * sinf(m_fAngle) + vTemp.y * cosf(m_fAngle);
	// 
	// 	// 위치 변환
	// 	m_vPoint[i] += m_tInfo.vPos;
	// 
	// }
	// 
	// // 포신
	// 
	// D3DXVECTOR3 vTemp = m_vOriginGunPoint;
	// 
	// vTemp -= { 400.f, 300.f, 0.f };
	// 
	// // 회전 변환
	// m_vGunPoint.x = vTemp.x * cosf(m_fAngle) - vTemp.y * sinf(m_fAngle);
	// m_vGunPoint.y = vTemp.x * sinf(m_fAngle) + vTemp.y * cosf(m_fAngle);
	// 
	// // 위치 변환
	// m_vGunPoint += m_tInfo.vPos;
#pragma endregion

	D3DXMATRIX		matScale, matRotZ, matTrans;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	for (int i = 0; i < 4; ++i)
	{
		m_vPoint[i] = m_vOriginPoint[i];

		m_vPoint[i] -= { 400.f, 300.f, 0.f};

		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &m_tInfo.matWorld);
	}
	
	// 포신
	m_vGunPoint = m_vOriginGunPoint;

	m_vGunPoint -= { 400.f, 300.f, 0.f};

	D3DXVec3TransformCoord(&m_vGunPoint, &m_vGunPoint, &m_tInfo.matWorld);

	Key_Input();
	
}

void CPlayer::Render(HDC hDC)
{
	MoveToEx(hDC, (int)m_vPoint[0].x, (int)m_vPoint[0].y, nullptr);

	for (int i = 0; i < 4; ++i)
	{
		LineTo(hDC, (int)m_vPoint[i].x, (int)m_vPoint[i].y);

		if (i > 0)
			continue;
		
		Ellipse(hDC, 
			int(m_vPoint[i].x - 5.f),
			int(m_vPoint[i].y - 5.f),
			int(m_vPoint[i].x + 5.f),
			int(m_vPoint[i].y + 5.f));
	}

	LineTo(hDC, (int)m_vPoint[0].x, (int)m_vPoint[0].y);

	// 포신

	MoveToEx(hDC, (int)m_tInfo.vPos.x, (int)m_tInfo.vPos.y, nullptr);
	LineTo(hDC, (int)m_vGunPoint.x, (int)m_vGunPoint.y);
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()
{
	if (GetAsyncKeyState('A'))
		m_fAngle -= D3DXToRadian(3.f);

	if (GetAsyncKeyState('D'))
		m_fAngle += D3DXToRadian(3.f);

	if (GetAsyncKeyState('W'))
	{
		// m_tInfo.vDir.x = m_tInfo.vLook.x * cosf(m_fAngle) - m_tInfo.vLook.y * sinf(m_fAngle);
		// m_tInfo.vDir.y = m_tInfo.vLook.x * sinf(m_fAngle) + m_tInfo.vLook.y * cosf(m_fAngle);

		D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);

		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;			 
	}

	if (GetAsyncKeyState('S'))
	{
		// m_tInfo.vDir.x = m_tInfo.vLook.x * cosf(m_fAngle) - m_tInfo.vLook.y * sinf(m_fAngle);
		// m_tInfo.vDir.y = m_tInfo.vLook.x * sinf(m_fAngle) + m_tInfo.vLook.y * cosf(m_fAngle);

		D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
		m_tInfo.vPos += m_tInfo.vDir * -m_fSpeed;
	}
}

