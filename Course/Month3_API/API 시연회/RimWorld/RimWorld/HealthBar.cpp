#include "pch.h"
#include "HealthBar.h"

#include "Pawn.h"

CHealthBar::CHealthBar()
{
}

CHealthBar::~CHealthBar()
{
    Release();
}

void CHealthBar::Initialize()
{
	m_tInfo.fCX = 80.f;
	m_tInfo.fCY = 10.f;

    m_eRenderID = RENDER_INUI;
}

int CHealthBar::Update()
{
    return CObj::Update();
}

void CHealthBar::Late_Update()
{
    if (CPawn* pPawn = dynamic_cast<CPawn*>(m_pTarget))
    {
        if (pPawn->Get_IsDead())
        {
            Set_Destroyed();
        }
    }

	Follow_Pawn();
}

void CHealthBar::Render(HDC hDC)
{
    // ���� ��ǥ�� ȭ�� ��ǥ�� ��ȯ
    POINT tTopLeft = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left, (float)m_tRect.top);
    POINT tBottomRight = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.right, (float)m_tRect.bottom);

    // ������ �귯�� ����
    HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
    // ������ �귯�� ����
    HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));

    // ���� HDC�� �⺻ �귯�ø� ����
    HBRUSH hDefaultBrush = (HBRUSH)SelectObject(hDC, hBlackBrush);

    // ������ �簢�� �׸��� (���)
    Rectangle(hDC, tTopLeft.x, tTopLeft.y, tBottomRight.x, tBottomRight.y);

    // HP ���� ���
    CPawn* pPawn = static_cast<CPawn*>(m_pTarget);
    float fHPRatio = 0.0f;

    try {
        float fMaxHp = pPawn->Get_MaxHP();
        if (fMaxHp == 0.f)
            throw fMaxHp;

        fHPRatio = pPawn->Get_HP() / fMaxHp;
    }
    catch (float) {
        fHPRatio = 0.f;
    }

    // HP ������ 0 ~ 1�� ����
    fHPRatio = max(0.f, min(1.f, fHPRatio));

    // ������ HP ���� ������ ��ǥ ���
    int iRedRight = tTopLeft.x + int((tBottomRight.x - tTopLeft.x) * fHPRatio);

    // ������ �簢�� �׸���
    SelectObject(hDC, hRedBrush);
    Rectangle(hDC, tTopLeft.x, tTopLeft.y, iRedRight, tBottomRight.y);

    // ���� �귯�÷� ����
    SelectObject(hDC, hDefaultBrush);

    // ���ҽ� ����
    DeleteObject(hRedBrush);
    DeleteObject(hBlackBrush);

}

void CHealthBar::Release()
{
	
}

void CHealthBar::Follow_Pawn()
{
	Set_Pos(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY + HELATHBAR_OFFSET);
}
