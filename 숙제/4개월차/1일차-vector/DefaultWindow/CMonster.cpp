#include "pch.h"
#include "CMonster.h"

CMonster::CMonster()
	:m_pPlayer(nullptr)
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
    // �÷��̾� ���� ��� (���� ����)
    m_tInfo.vDir = m_pPlayer->Get_Info().vPos - m_tInfo.vPos;
    float fDiagonal = sqrtf(m_tInfo.vDir.x * m_tInfo.vDir.x + m_tInfo.vDir.y * m_tInfo.vDir.y + m_tInfo.vDir.z * m_tInfo.vDir.z);
    if (fDiagonal != 0.0f) 
    {
        m_tInfo.vDir.x /= fDiagonal;
        m_tInfo.vDir.y /= fDiagonal;
        m_tInfo.vDir.z /= fDiagonal;
    }

    // ���� �������� �ü� õõ�� ����
    float fLerpSpeed = 0.05f; // ���� �ӵ� (0.0 ~ 1.0)
    m_tInfo.vLook.x = m_tInfo.vLook.x + fLerpSpeed * (m_tInfo.vDir.x - m_tInfo.vLook.x);
    m_tInfo.vLook.y = m_tInfo.vLook.y + fLerpSpeed * (m_tInfo.vDir.y - m_tInfo.vLook.y);
    m_tInfo.vLook.z = m_tInfo.vLook.z + fLerpSpeed * (m_tInfo.vDir.z - m_tInfo.vLook.z);

    // �ü� ���� ����ȭ
    float fLookLength = sqrtf(m_tInfo.vLook.x * m_tInfo.vLook.x + m_tInfo.vLook.y * m_tInfo.vLook.y + m_tInfo.vLook.z * m_tInfo.vLook.z);
    if (fLookLength != 0.0f) 
    {
        m_tInfo.vLook.x /= fLookLength;
        m_tInfo.vLook.y /= fLookLength;
        m_tInfo.vLook.z /= fLookLength;
    }

    // �ٶ󺸴� �������� �̵�
    m_tInfo.vPos += m_tInfo.vLook * m_fSpeed;
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

	// �ؽ�Ʈ ���
	//������ �ü��� �÷��̾� ������ �����ϸ�
	float fDotProduct = m_tInfo.vDir.x * m_tInfo.vLook.x + m_tInfo.vDir.y * m_tInfo.vLook.y + m_tInfo.vDir.z * m_tInfo.vLook.z;
	std::wstring strText = L"fDotProduct:" + to_wstring(fDotProduct);
	TextOut(hDC, (int)m_tInfo.vPos.x - 40, (int)m_tInfo.vPos.y - 70, strText.c_str(), strText.length());
}

void CMonster::Release()
{
}


// ������ �̿��Ϸ��� �� ���� ���Ͱ� �־�� ��
