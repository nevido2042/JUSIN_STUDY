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
	//시선벡터, 플레이어벡터를 가지고 사잇각을 구하고
	//사잇각을 활용해서 이동하기

	//플레이어 벡터
	m_tInfo.vDir = m_pPlayer->Get_Info().vPos - m_tInfo.vPos;

	//방향벡터 정규화
	float fWidth = m_tInfo.vPos.x - m_pPlayer->Get_Info().vPos.x;
	float fHeight = m_tInfo.vPos.y - m_pPlayer->Get_Info().vPos.y;
	float Diagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);
	m_tInfo.vDir.x /= Diagonal;
	m_tInfo.vDir.y /= Diagonal;
	m_tInfo.vDir.z /= Diagonal;
	
	//시선벡터, 방향벡터 내적
	float fDot = m_tInfo.vLook.x * m_tInfo.vDir.x + m_tInfo.vLook.y * m_tInfo.vDir.y + m_tInfo.vLook.z * m_tInfo.vDir.z;

	//라디안 각도 구하기
	m_fRadian = acos(fDot);

	if (m_pPlayer->Get_Info().vPos.y < m_tInfo.vPos.y)
	{
		m_fRadian *= -1.f;
	}

	//각도로 이동하기
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

	// 텍스트 출력
	//몬스터의 시선과 플레이어 방향을 내적하면
	float fDotProduct = m_tInfo.vDir.x * m_tInfo.vLook.x + m_tInfo.vDir.y * m_tInfo.vLook.y + m_tInfo.vDir.z * m_tInfo.vLook.z;
	std::wstring strText = L"fDotProduct:" + to_wstring(fDotProduct);
	TextOut(hDC, (int)m_tInfo.vPos.x - 40, (int)m_tInfo.vPos.y - 70, strText.c_str(), strText.length());

	// 텍스트 출력
	//몬스터의 시선과 플레이어 방향을 내적하면
	strText = L"m_fRadian:" + to_wstring(m_fRadian);
	TextOut(hDC, (int)m_tInfo.vPos.x - 40, (int)m_tInfo.vPos.y - 100, strText.c_str(), strText.length());

}

void CMonster::Release()
{
}


// 내적을 이용하려면 두 방향 벡터가 있어야 함
