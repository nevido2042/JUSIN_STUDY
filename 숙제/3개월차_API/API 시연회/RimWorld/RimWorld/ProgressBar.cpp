#include "pch.h"
#include "ProgressBar.h"

CProgressBar::CProgressBar()
    :m_fCurValue(0.f), m_fMaxValue(0.f)
{
}

CProgressBar::~CProgressBar()
{
	Release();
}

void CProgressBar::Set_Value(float _fCurValue, float _fMaxValue)
{
    m_fCurValue = _fCurValue;
    m_fMaxValue = _fMaxValue;
}

void CProgressBar::Initialize()
{
    m_fMaxValue = 1.f;//예외 발생하지 않도록

    m_tInfo.fCX = 200.f;
    m_tInfo.fCY = 30.f;

    m_eRenderID = RENDER_UI;
}

void CProgressBar::Late_Update()
{
}

void CProgressBar::Render(HDC hDC)
{
    // 빨간색 펜과 브러시 생성
    HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 255, 0));   // 빨간색 브러시
    // 검은색 펜과 브러시 생성
    HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));   // 검은색 브러시
    // 현재 HDC의 기본 펜과 브러시를 저장
    HBRUSH hDefaultBrush = (HBRUSH)SelectObject(hDC, hBlackBrush);
    // 검은색 사각형 그리기 (위치 변경)
    Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

    float  fRatio(0.f);
    try
    {
        //0 나누기 확인
        if (m_fMaxValue == 0.f)
        {
            throw m_fMaxValue;
        }

        fRatio = m_fCurValue / m_fMaxValue;
    }
    catch (float)
    {
        fRatio = 0.f;
    }

    //비율 0.f~ 1.f 제한
    if (fRatio < 0.f)
    {
        fRatio = 0.f;
    }
    else if (fRatio > 1.f)
    {
        fRatio = 1.f;
    }

    //HP바의 빨간 부분의 Right를 재설정
    int iRedRight = m_tRect.left + int((m_tRect.right - m_tRect.left) * fRatio);
    // 빨간색 사각형 그리기
    SelectObject(hDC, hRedBrush);
    Rectangle(hDC, m_tRect.left, m_tRect.top, iRedRight, m_tRect.bottom);

    // 원래 펜과 브러시로 복원
    SelectObject(hDC, hDefaultBrush);

    // 리소스 해제
    DeleteObject(hRedBrush);
    DeleteObject(hBlackBrush);
}

void CProgressBar::Release()
{
}
