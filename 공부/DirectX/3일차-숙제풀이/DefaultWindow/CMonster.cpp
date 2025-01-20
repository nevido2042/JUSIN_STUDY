#include "pch.h"
#include "CMonster.h"

#define SIZE 50.f

CMonster::CMonster():m_fAngle(0.f), m_fOrbitAngle(0.f)
{
}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	m_tInfo.vPos = { 100.f, 100.f, 0.f };
	m_fSpeed = 2.f;	

	m_vOriginPoint[0] = { 0.f, -SIZE, 0.f };
	m_vOriginPoint[1] = { SIZE, SIZE, 0.f };
	m_vOriginPoint[2] = { -SIZE, SIZE, 0.f };

	//m_fAngle = D3DXToRadian(10.f);
}

void CMonster::Update()
{
	//m_tInfo.vPos.x += 1.f; // 자체 이동
	m_fAngle += D3DXToRadian(10.f); // 자체 회전 각도
	m_fOrbitAngle += D3DXToRadian(1.f); // 공전 각도

	D3DXMATRIX matScale, matRotZ, matTrans, matOrbitRot, matOrbitTrans;

	// 스케일 행렬 (크기 조정)
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	// 자체 회전 행렬 (Z축 기준 회전)
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);

	// 자체 위치 이동 행렬
	D3DXMatrixTranslation(&matTrans, -100.f, 0.f, 0.f);

	// 공전 중심 위치 (공전 중심 좌표)
	D3DXVECTOR3 vOrbitCenter = m_pPlayer->Get_Info().vPos; // 공전 중심 좌표

	// 공전 회전 행렬 (Z축 기준 회전)
	D3DXMatrixRotationZ(&matOrbitRot, m_fOrbitAngle);

	// 공전 이동 행렬 (공전 중심으로의 이동)
	D3DXMatrixTranslation(&matOrbitTrans, vOrbitCenter.x, vOrbitCenter.y, vOrbitCenter.z);

	m_tInfo.matWorld = matScale * matRotZ * matTrans * matOrbitRot * matOrbitTrans;
	// 점 변환
	for (int i = 0; i < 3; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vOriginPoint[i], &m_tInfo.matWorld);
	}

	//D3DXVec3TransformCoord(&m_tInfo.vPos, &m_tInfo.vPos, &m_tInfo.matWorld);
}

void CMonster::Render(HDC hDC)
{
	//Ellipse(hDC,
	//	int(m_tInfo.vPos.x - 50.f),
	//	int(m_tInfo.vPos.y - 50.f),
	//	int(m_tInfo.vPos.x + 50.f),
	//	int(m_tInfo.vPos.y + 50.f));

	MoveToEx(hDC, (int)m_vPoint[0].x, (int)m_vPoint[0].y, nullptr);
	LineTo(hDC, (int)m_vPoint[1].x, (int)m_vPoint[1].y);
	LineTo(hDC, (int)m_vPoint[2].x, (int)m_vPoint[2].y);
	LineTo(hDC, (int)m_vPoint[0].x, (int)m_vPoint[0].y);
}

void CMonster::Release()
{
}


// 내적을 이용하려면 두 방향 벡터가 있어야 함
