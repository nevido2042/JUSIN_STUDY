#include "pch.h"
#include "HealthBar.h"
#include "ScrollMgr.h"
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
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();


	return OBJ_NOEVENT;
}

void CHealthBar::Late_Update()
{
	Follow_Pawn();
}

void CHealthBar::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // 빨간색 펜과 브러시 생성
    HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 0, 0));   // 빨간색 브러시
    // 검은색 펜과 브러시 생성
    HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));   // 검은색 브러시
    // 현재 HDC의 기본 펜과 브러시를 저장
    HBRUSH hDefaultBrush = (HBRUSH)SelectObject(hDC, hBlackBrush);
    // 검은색 사각형 그리기 (위치 변경)
    Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

    //HP비율
    CPawn* pPawn = static_cast<CPawn*>(m_pTarget);
    float  fHPRatio(0.f);
    try
    {
        //0 나누기 확인
        float fMaxHp = pPawn->Get_MaxHP();
        if (fMaxHp == 0.f)
        {
            throw fMaxHp;
        }

        fHPRatio = pPawn->Get_HP() / pPawn->Get_MaxHP();
    }
    catch(float)
    {
        //최대체력이 0이면 체력바가 길어지는 버그나도록
        fHPRatio = 0.f;
    }

    //체력바 비율 0.f~ 1.f 제한
    if (fHPRatio < 0.f)
    {
        fHPRatio = 0.f;
    }
    else if (fHPRatio > 1.f)
    {
        fHPRatio = 1.f;
    }
    
    //HP바의 빨간 부분의 Right를 재설정
    int iRedRight = m_tRect.left + int((m_tRect.right - m_tRect.left) * fHPRatio);
    // 빨간색 사각형 그리기
    SelectObject(hDC, hRedBrush);
    Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, iRedRight + iScrollX, m_tRect.bottom + iScrollY);

    // 원래 펜과 브러시로 복원
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
