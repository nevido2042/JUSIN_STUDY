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
    // 플레이어 방향 계산 (단위 벡터)
    m_tInfo.vDir = m_pPlayer->Get_Info().vPos - m_tInfo.vPos;
    float fDiagonal = sqrtf(m_tInfo.vDir.x * m_tInfo.vDir.x + m_tInfo.vDir.y * m_tInfo.vDir.y + m_tInfo.vDir.z * m_tInfo.vDir.z);
    if (fDiagonal != 0.0f) 
    {
        m_tInfo.vDir.x /= fDiagonal;
        m_tInfo.vDir.y /= fDiagonal;
        m_tInfo.vDir.z /= fDiagonal;
    }

    // 선형 보간으로 시선 천천히 변경
    float fLerpSpeed = 0.05f; // 보간 속도 (0.0 ~ 1.0)
    m_tInfo.vLook.x = m_tInfo.vLook.x + fLerpSpeed * (m_tInfo.vDir.x - m_tInfo.vLook.x);
    m_tInfo.vLook.y = m_tInfo.vLook.y + fLerpSpeed * (m_tInfo.vDir.y - m_tInfo.vLook.y);
    m_tInfo.vLook.z = m_tInfo.vLook.z + fLerpSpeed * (m_tInfo.vDir.z - m_tInfo.vLook.z);

    // 시선 벡터 정규화
    float fLookLength = sqrtf(m_tInfo.vLook.x * m_tInfo.vLook.x + m_tInfo.vLook.y * m_tInfo.vLook.y + m_tInfo.vLook.z * m_tInfo.vLook.z);
    if (fLookLength != 0.0f) 
    {
        m_tInfo.vLook.x /= fLookLength;
        m_tInfo.vLook.y /= fLookLength;
        m_tInfo.vLook.z /= fLookLength;
    }

    // 바라보는 방향으로 이동
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

	// 텍스트 출력
	//몬스터의 시선과 플레이어 방향을 내적하면
	float fDotProduct = m_tInfo.vDir.x * m_tInfo.vLook.x + m_tInfo.vDir.y * m_tInfo.vLook.y + m_tInfo.vDir.z * m_tInfo.vLook.z;
	std::wstring strText = L"fDotProduct:" + to_wstring(fDotProduct);
	TextOut(hDC, (int)m_tInfo.vPos.x - 40, (int)m_tInfo.vPos.y - 70, strText.c_str(), strText.length());
}

void CMonster::Release()
{
}


// 내적을 이용하려면 두 방향 벡터가 있어야 함
