#include "pch.h"
#include "CMonster.h"

CMonster::CMonster()
	:m_pPlayer(nullptr), m_fRadian(0.f)
{
}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	m_tInfo.vPos = { 100.f, 100.f, 0.f };
	m_fSpeed = 3.f;
	
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

}

void CMonster::Update()
{
	//�ü�����, �÷��̾�͸� ������ ���հ��� ���ϰ�
	//���հ��� Ȱ���ؼ� �̵��ϱ�

	//�÷��̾� ����
	m_tInfo.vDir = m_pPlayer->Get_Info().vPos - m_tInfo.vPos;

	//���⺤�� ����ȭ
	float fWidth = m_tInfo.vPos.x - m_pPlayer->Get_Info().vPos.x;
	float fHeight = m_tInfo.vPos.y - m_pPlayer->Get_Info().vPos.y;
	float Diagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);
	m_tInfo.vDir.x /= Diagonal;
	m_tInfo.vDir.y /= Diagonal;
	m_tInfo.vDir.z /= Diagonal;
	
	//�ü�����, ���⺤�� ����
	float fDot = m_tInfo.vLook.x * m_tInfo.vDir.x + m_tInfo.vLook.y * m_tInfo.vDir.y + m_tInfo.vLook.z * m_tInfo.vDir.z;

	//���� ���� ���ϱ�
	m_fRadian = acos(fDot);

	if (m_pPlayer->Get_Info().vPos.y < m_tInfo.vPos.y)
	{
		m_fRadian *= -1.f;
	}

	//������ �̵��ϱ�
	m_tInfo.vPos.x += cos(m_fRadian) * m_fSpeed;
	m_tInfo.vPos.y += sin(m_fRadian) * m_fSpeed;
}

void CMonster::Render(HDC hDC)
{
	Ellipse(hDC,
		int(m_tInfo.vPos.x - 50.f),
		int(m_tInfo.vPos.y - 50.f),
		int(m_tInfo.vPos.x + 50.f),
		int(m_tInfo.vPos.y + 50.f));

	MoveToEx(hDC, (int)m_tInfo.vPos.x, (int)m_tInfo.vPos.y, nullptr);
	float fLookLineLength = 100.f;
	int iLookX = int(m_tInfo.vPos.x + m_tInfo.vLook.x * fLookLineLength);
	int iLookY = int(m_tInfo.vPos.y + m_tInfo.vLook.y * fLookLineLength);
	LineTo(hDC, iLookX, iLookY);

	MoveToEx(hDC, (int)m_tInfo.vPos.x, (int)m_tInfo.vPos.y, nullptr);
	LineTo(hDC, (int)m_pPlayer->Get_Info().vPos.x, (int)m_pPlayer->Get_Info().vPos.y);

	// �ؽ�Ʈ ���
	//������ �ü��� �÷��̾� ������ �����ϸ�
	float fDotProduct = m_tInfo.vDir.x * m_tInfo.vLook.x + m_tInfo.vDir.y * m_tInfo.vLook.y + m_tInfo.vDir.z * m_tInfo.vLook.z;
	std::wstring strText = L"fDotProduct:" + to_wstring(fDotProduct);
	TextOut(hDC, (int)m_tInfo.vPos.x - 40, (int)m_tInfo.vPos.y - 70, strText.c_str(), strText.length());

	// �ؽ�Ʈ ���
	//������ �ü��� �÷��̾� ������ �����ϸ�
	strText = L"m_fRadian:" + to_wstring(m_fRadian);
	TextOut(hDC, (int)m_tInfo.vPos.x - 40, (int)m_tInfo.vPos.y - 100, strText.c_str(), strText.length());

}

void CMonster::Release()
{
}


// ������ �̿��Ϸ��� �� ���� ���Ͱ� �־�� ��
