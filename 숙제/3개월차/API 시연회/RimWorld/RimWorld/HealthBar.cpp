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
    // 월드 좌표를 화면 좌표로 변환
    POINT tTopLeft = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left, (float)m_tRect.top);
    POINT tBottomRight = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.right, (float)m_tRect.bottom);

    // 빨간색 브러시 생성
    HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
    // 검은색 브러시 생성
    HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));

    // 현재 HDC의 기본 브러시를 저장
    HBRUSH hDefaultBrush = (HBRUSH)SelectObject(hDC, hBlackBrush);

    // 검은색 사각형 그리기 (배경)
    Rectangle(hDC, tTopLeft.x, tTopLeft.y, tBottomRight.x, tBottomRight.y);

    // HP 비율 계산
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

    // HP 비율을 0 ~ 1로 제한
    fHPRatio = max(0.f, min(1.f, fHPRatio));

    // 빨간색 HP 바의 오른쪽 좌표 계산
    int iRedRight = tTopLeft.x + int((tBottomRight.x - tTopLeft.x) * fHPRatio);

    // 빨간색 사각형 그리기
    SelectObject(hDC, hRedBrush);
    Rectangle(hDC, tTopLeft.x, tTopLeft.y, iRedRight, tBottomRight.y);

    // 원래 브러시로 복원
    SelectObject(hDC, hDefaultBrush);

    // 리소스 해제
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
