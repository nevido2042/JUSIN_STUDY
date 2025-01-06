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
    m_fMaxValue = 1.f;//���� �߻����� �ʵ���

    m_tInfo.fCX = 200.f;
    m_tInfo.fCY = 30.f;

    m_eRenderID = RENDER_UI;
}

void CProgressBar::Late_Update()
{
}

void CProgressBar::Render(HDC hDC)
{
    // ������ ��� �귯�� ����
    HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 255, 0));   // ������ �귯��
    // ������ ��� �귯�� ����
    HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));   // ������ �귯��
    // ���� HDC�� �⺻ ��� �귯�ø� ����
    HBRUSH hDefaultBrush = (HBRUSH)SelectObject(hDC, hBlackBrush);
    // ������ �簢�� �׸��� (��ġ ����)
    Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

    float  fRatio(0.f);
    try
    {
        //0 ������ Ȯ��
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

    //���� 0.f~ 1.f ����
    if (fRatio < 0.f)
    {
        fRatio = 0.f;
    }
    else if (fRatio > 1.f)
    {
        fRatio = 1.f;
    }

    //HP���� ���� �κ��� Right�� �缳��
    int iRedRight = m_tRect.left + int((m_tRect.right - m_tRect.left) * fRatio);
    // ������ �簢�� �׸���
    SelectObject(hDC, hRedBrush);
    Rectangle(hDC, m_tRect.left, m_tRect.top, iRedRight, m_tRect.bottom);

    // ���� ��� �귯�÷� ����
    SelectObject(hDC, hDefaultBrush);

    // ���ҽ� ����
    DeleteObject(hRedBrush);
    DeleteObject(hBlackBrush);
}

void CProgressBar::Release()
{
}
