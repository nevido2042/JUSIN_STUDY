#include "pch.h"
#include "ScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr() : m_fScrollX(0.f), m_fScrollY(0.f), m_bMoveLerp(false), m_fMoveX(0.f), m_fMoveY(0.f)
{
}

CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Scroll_Lock()
{
	float fMinX = TILECX * TILEX * 0.2f;
	float fMinY = TILECY * TILEY * 0.2f;

	float fMaxX = TILECX * TILEX * 1.2f;
	float fMaxY = TILECY * TILEY * 1.2f;

	if (fMinX < m_fScrollX)
		m_fScrollX = fMinX;

	if (WINCX - fMaxX > m_fScrollX)
		m_fScrollX = WINCX - fMaxX;

	if (fMinY < m_fScrollY)
		m_fScrollY = fMinY;

	if (WINCY - fMaxY > m_fScrollY)
		m_fScrollY = WINCY - fMaxY;
}

void CScrollMgr::Late_Update()
{
	if (m_bMoveLerp)
	{
		// 목표 위치 계산
		float fTargetX = -m_fMoveX + WINCX * 0.5f;
		float fTargetY = -m_fMoveY + WINCY * 0.5f;

		// 현재 위치 가져오기
		float fCurrentX = CScrollMgr::Get_Instance()->Get_ScrollX();
		float fCurrentY = CScrollMgr::Get_Instance()->Get_ScrollY();

		// 보간 비율 설정 (0.0f ~ 1.0f)
		float fLerpFactor = 0.1f; // 0.1f = 느리게 이동, 1.0f = 즉시 이동

		// LERP 계산
		float fNewX = fCurrentX + (fTargetX - fCurrentX) * fLerpFactor;
		float fNewY = fCurrentY + (fTargetY - fCurrentY) * fLerpFactor;

		// 새로운 위치 설정
		CScrollMgr::Get_Instance()->Set_ScrollX(fNewX);
		CScrollMgr::Get_Instance()->Set_ScrollY(fNewY);

		// 거리 계산 (유클리드 거리)
		float fDeltaX = fTargetX - fCurrentX;
		float fDeltaY = fTargetY - fCurrentY;
		float fDistance = sqrt(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

		// 목표 위치에 가까워지면 리턴
		if (fDistance < 1.0f) { // 1.0f는 임계값(허용 오차)으로 조정 가능
			m_bMoveLerp = false;
		}
	}
}

void CScrollMgr::Move_To_Lerp(float _fX, float _fY)
{
	m_fMoveX = _fX;
	m_fMoveY = _fY;
	m_bMoveLerp = true;
}
