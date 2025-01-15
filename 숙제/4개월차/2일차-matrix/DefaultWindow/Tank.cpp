#include "pch.h"
#include "Tank.h"
#include "KeyMgr.h"

CTank::CTank()
	:m_fSize(0.f), m_fGunLength(0.f), m_fBodyAngle(0.f), m_fTurretAngle(0.f)
{
	ZeroMemory(&m_tTankBody, sizeof(m_tTankBody));
	ZeroMemory(&m_tGunEnd, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_matGunWorld, sizeof(D3DXMATRIX));
}

CTank::~CTank()
{
	Release();
}

void CTank::Initialize()
{
	m_tInfo.vPos.x = (float)WINCX * 0.5f;
	m_tInfo.vPos.y = (float)WINCY * 0.5f;

	//m_tInfo.vLook = { 0, -1, 0 };	//���� ����
	//m_tInfo.vDir = { 0, -1, 0 };	//��ü ����

	m_fSize = 50.f;

	m_fSpeed = 1.f;

	m_fGunLength = 100.f;

	//��ü �ʱ� ��ġ
	m_tTankBody[LF] = { -m_fSize, -m_fSize, 0.f };
	m_tTankBody[RF] = { m_fSize, -m_fSize, 0.f };
	m_tTankBody[RR] = { m_fSize, m_fSize, 0.f };
	m_tTankBody[LR] = { -m_fSize, m_fSize, 0.f };

	//�� �ʱ� ��ġ
	m_tGunEnd = { 0.f , -m_fGunLength, 0.f };
}

void CTank::Update()
{
	Input_Key();

	Update_WorldMatrix();
	Update_WorldMatrix_Gun();
}

void CTank::Render(HDC hDC)
{
	//��ü
	D3DXVECTOR3 vTankBody_World[4];
	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vTankBody_World[i], &m_tTankBody[i], &m_tInfo.matWorld);
	}
	MoveToEx(hDC, vTankBody_World[LF].x, vTankBody_World[LF].y, nullptr);
	LineTo(hDC, vTankBody_World[RF].x, vTankBody_World[RF].y);
	LineTo(hDC, vTankBody_World[RR].x, vTankBody_World[RR].y);
	LineTo(hDC, vTankBody_World[LR].x, vTankBody_World[LR].y);
	LineTo(hDC, vTankBody_World[LF].x, vTankBody_World[LF].y);

	//������
	Ellipse(hDC, vTankBody_World[RF].x - 10, vTankBody_World[RF].y - 10, vTankBody_World[RF].x + 10, vTankBody_World[RF].y + 10);
	Ellipse(hDC, vTankBody_World[LF].x - 10, vTankBody_World[LF].y - 10, vTankBody_World[LF].x + 10, vTankBody_World[LF].y + 10);

	//����
	D3DXVECTOR3 vGunEnd_World;
	D3DXVec3TransformCoord(&vGunEnd_World, &m_tGunEnd, &m_matGunWorld);
	MoveToEx(hDC, m_tInfo.vPos.x, m_tInfo.vPos.y, nullptr);
	LineTo(hDC, vGunEnd_World.x, vGunEnd_World.y);

	TCHAR buffer[64]; // ������ ������ ���ڿ� ����
	// ���� ���� ������ ��ȯ
	float degree = D3DXToDegree(m_fBodyAngle);
	// ���ڿ��� ��ȯ�Ͽ� ��� �غ�
	_stprintf_s(buffer, _T("Body Angle: %.2f��"), degree);
	// ȭ�鿡 ��� (10, 10 ��ġ)
	TextOut(hDC, 10, 10, buffer, _tcslen(buffer));

	degree = D3DXToDegree(m_fTurretAngle);
	// ���ڿ��� ��ȯ�Ͽ� ��� �غ�
	_stprintf_s(buffer, _T("Turret Angle: %.2f��"), degree);
	// ȭ�鿡 ��� (10, 10 ��ġ)
	TextOut(hDC, 10, 30, buffer, _tcslen(buffer));
}

void CTank::Release()
{
}

void CTank::Input_Key()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
	{
		m_tInfo.vPos.y -= cos(m_fBodyAngle) * m_fSpeed;
		m_tInfo.vPos.x += sin(m_fBodyAngle) * m_fSpeed;
	}
	
	if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
	{
		m_tInfo.vPos.y += cos(m_fBodyAngle) * m_fSpeed;
		m_tInfo.vPos.x -= sin(m_fBodyAngle) * m_fSpeed;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
	{
		m_fBodyAngle -= D3DXToRadian(m_fSpeed);
		m_fTurretAngle -= D3DXToRadian(m_fSpeed);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
	{
		m_fBodyAngle += D3DXToRadian(m_fSpeed);
		m_fTurretAngle += D3DXToRadian(m_fSpeed);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_fTurretAngle -= D3DXToRadian(m_fSpeed);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_fTurretAngle += D3DXToRadian(m_fSpeed);
	}
}

void CTank::Update_WorldMatrix()
{	
	//ũ��
	D3DXMATRIX		matScale;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	//ȸ��
	D3DXMATRIX		matRotZ, matRevZ;
	D3DXMatrixRotationZ(&matRotZ, m_fBodyAngle);

	//�̵�
	D3DXMATRIX		matTrans;
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	
	m_tInfo.matWorld = matScale * matRotZ * matTrans;

}

void CTank::Update_WorldMatrix_Gun()
{
	//ũ��
	D3DXMATRIX		matScale;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	//ȸ��
	D3DXMATRIX		matRotZ, matRevZ;
	D3DXMatrixRotationZ(&matRotZ, m_fTurretAngle);

	//�̵�
	D3DXMATRIX		matTrans;
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_matGunWorld = matScale * matRotZ * matTrans;
}
