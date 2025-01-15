#include "pch.h"
#include "Tank.h"
#include "KeyMgr.h"

CTank::CTank()
	:m_fSize(0.f), m_fGunLength(0.f), m_fAngle(0.f)
{
	ZeroMemory(&m_tTankBody, sizeof(m_tTankBody));
}

CTank::~CTank()
{
	Release();
}

void CTank::Initialize()
{
	m_tInfo.vPos.x = (float)WINCX * 0.5f;
	m_tInfo.vPos.y = (float)WINCY * 0.5f;

	m_tInfo.vLook = { 0, -1, 0 };	//���� ����
	//m_tInfo.vDir = { 0, -1, 0 };	//��ü ����

	m_fSize = 50.f;

	m_fSpeed = 1.f;

	m_fGunLength = 100.f;

	m_tTankBody[LF] = { -m_fSize, -m_fSize, 0.f };
	m_tTankBody[RF] = { m_fSize, -m_fSize, 0.f };
	m_tTankBody[RR] = { m_fSize, m_fSize, 0.f };
	m_tTankBody[LR] = { -m_fSize, m_fSize, 0.f };
}

void CTank::Update()
{
	Input_Key();

	Update_WorldMatrix();
}

void CTank::Render(HDC hDC)
{
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

	Ellipse(hDC, vTankBody_World[RF].x - 10, vTankBody_World[RF].y - 10, vTankBody_World[RF].x + 10, vTankBody_World[RF].y + 10);
	Ellipse(hDC, vTankBody_World[LF].x - 10, vTankBody_World[LF].y - 10, vTankBody_World[LF].x + 10, vTankBody_World[LF].y + 10);

	MoveToEx(hDC, (int)m_tInfo.vPos.x, (int)m_tInfo.vPos.y, nullptr);
	//���� �� ��
	float fX = (m_tInfo.vPos.x + m_tInfo.vLook.x * m_fGunLength);
	float fY = (m_tInfo.vPos.y + m_tInfo.vLook.y * m_fGunLength);
	LineTo(hDC, (int)fX, (int)fY);

	TCHAR buffer[64]; // ������ ������ ���ڿ� ����
	// ���� ���� ������ ��ȯ
	float degree = D3DXToDegree(m_fAngle);
	// ���ڿ��� ��ȯ�Ͽ� ��� �غ�
	_stprintf_s(buffer, _T("Angle: %.2f��"), degree);
	// ȭ�鿡 ��� (10, 10 ��ġ)
	TextOut(hDC, 10, 10, buffer, _tcslen(buffer));
}

void CTank::Release()
{
}

void CTank::Input_Key()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
	{
		m_tInfo.vPos.y -= cos(m_fAngle) * m_fSpeed;
		m_tInfo.vPos.x += sin(m_fAngle) * m_fSpeed;
	}
	
	if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
	{
		m_tInfo.vPos.y += cos(m_fAngle) * m_fSpeed;
		m_tInfo.vPos.x -= sin(m_fAngle) * m_fSpeed;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
	{
		m_fAngle -= D3DXToRadian(m_fSpeed);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
	{
		m_fAngle += D3DXToRadian(m_fSpeed);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		D3DXMATRIX matRotZ;
		// ȸ�� ��ȯ ����� ����� �Լ�
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-5.f));
		// ���Ϳ� ����� ������ ��������ִ� �Լ�
		D3DXVec3TransformCoord(&m_tInfo.vLook, &m_tInfo.vLook, &matRotZ);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		D3DXMATRIX matRotZ;
		// ȸ�� ��ȯ ����� ����� �Լ�
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(5.f));
		// ���Ϳ� ����� ������ ��������ִ� �Լ�
		D3DXVec3TransformCoord(&m_tInfo.vLook, &m_tInfo.vLook, &matRotZ);
	}
}

void CTank::Update_WorldMatrix()
{	
	//ũ��
	D3DXMATRIX		matScale;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	//ȸ��
	D3DXMATRIX		matRotZ, matRevZ;
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);

	//�̵�
	D3DXMATRIX		matTrans;
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	
	m_tInfo.matWorld = matScale * matRotZ * matTrans;
}
