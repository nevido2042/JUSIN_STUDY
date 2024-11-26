#include "pch.h"
#include "Monster.h"

CMonster::CMonster()
	:m_bDead(false), m_bRight(false), m_iHP(0)
{
}

void CMonster::Initialize()
{
	m_tInfo = { WINCX * 0.5f, WINCY * 0.2f, 75.f, 75.f };
	m_fSpeed = 5.f;
	m_iHP = 10;
	m_bRight = true;
	m_bDead = false;
}

void CMonster::Update()
{
	if (m_bDead)
	{
		return;
	}

	if (m_bRight)
	{
		m_tInfo.fX += m_fSpeed;

		if (MINI_WINCX_RIGHT < Get_Info().fX)
		{
			m_bRight = false;
		}
	}
	else
	{
		m_tInfo.fX -= m_fSpeed;

		if (MINI_WINCX_LEFT > Get_Info().fX)
		{
			m_bRight = true;
		}
	}
	// CObj::Update_Rect();
	__super::Update_Rect();

	// BOOL IntersectRect(�浹�� ��Ʈ�� ���� �߻��� ��Ʈ�� ������ ������ �ּ�, ��Ʈ �ּ�1, ��Ʈ �ּ�2)
	// IntersectRect(&Get_Rect(),)
}

void CMonster::Render(HDC hDC)
{
	if (m_bDead)
	{
		return;
	}

	Ellipse(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tRect.right,
		m_tRect.bottom);

	TCHAR		szBuf[32] = L"";
	//wsprintf(szBuf, L"Bullet : %I64u", m_BulletList.size());
	// wsprintf(szBuf, L"Bullet : %f", 3.141592f);	// �Ҽ��� �ڸ��� ����� �Ұ���

	swprintf_s(szBuf, L"HP : %d", m_iHP);	// ��� ���� ���� ���� �Լ�
	TextOut(hDC, int(Get_Info().fX - Get_Info().fCX * 0.25f) , int(Get_Info().fY), szBuf, lstrlen(szBuf));
}

void CMonster::Release()
{
}

void CMonster::Add_HP(int _iAmount)
{
	m_iHP += _iAmount;

	if (m_iHP <= 0)
	{
		m_bDead = true;
	}
}
