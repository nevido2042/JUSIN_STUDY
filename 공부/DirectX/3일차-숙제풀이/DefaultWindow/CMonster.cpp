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
	//m_tInfo.vPos.x += 1.f; // ��ü �̵�
	m_fAngle += D3DXToRadian(10.f); // ��ü ȸ�� ����
	m_fOrbitAngle += D3DXToRadian(1.f); // ���� ����

	D3DXMATRIX matScale, matRotZ, matTrans, matOrbitRot, matOrbitTrans;

	// ������ ��� (ũ�� ����)
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	// ��ü ȸ�� ��� (Z�� ���� ȸ��)
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);

	// ��ü ��ġ �̵� ���
	D3DXMatrixTranslation(&matTrans, -100.f, 0.f, 0.f);

	// ���� �߽� ��ġ (���� �߽� ��ǥ)
	D3DXVECTOR3 vOrbitCenter = m_pPlayer->Get_Info().vPos; // ���� �߽� ��ǥ

	// ���� ȸ�� ��� (Z�� ���� ȸ��)
	D3DXMatrixRotationZ(&matOrbitRot, m_fOrbitAngle);

	// ���� �̵� ��� (���� �߽������� �̵�)
	D3DXMatrixTranslation(&matOrbitTrans, vOrbitCenter.x, vOrbitCenter.y, vOrbitCenter.z);

	m_tInfo.matWorld = matScale * matRotZ * matTrans * matOrbitRot * matOrbitTrans;
	// �� ��ȯ
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


// ������ �̿��Ϸ��� �� ���� ���Ͱ� �־�� ��
