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
		// ��ǥ ��ġ ���
		float fTargetX = -m_fMoveX + WINCX * 0.5f;
		float fTargetY = -m_fMoveY + WINCY * 0.5f;

		// ���� ��ġ ��������
		float fCurrentX = CScrollMgr::Get_Instance()->Get_ScrollX();
		float fCurrentY = CScrollMgr::Get_Instance()->Get_ScrollY();

		// ���� ���� ���� (0.0f ~ 1.0f)
		float fLerpFactor = 0.1f; // 0.1f = ������ �̵�, 1.0f = ��� �̵�

		// LERP ���
		float fNewX = fCurrentX + (fTargetX - fCurrentX) * fLerpFactor;
		float fNewY = fCurrentY + (fTargetY - fCurrentY) * fLerpFactor;

		// ���ο� ��ġ ����
		CScrollMgr::Get_Instance()->Set_ScrollX(fNewX);
		CScrollMgr::Get_Instance()->Set_ScrollY(fNewY);

		// �Ÿ� ��� (��Ŭ���� �Ÿ�)
		float fDeltaX = fTargetX - fCurrentX;
		float fDeltaY = fTargetY - fCurrentY;
		float fDistance = sqrt(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

		// ��ǥ ��ġ�� ��������� ����
		if (fDistance < 1.0f) { // 1.0f�� �Ӱ谪(��� ����)���� ���� ����
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
